#ifndef _GAPIDENTIFIER_H_
#define _GAPIDENTIFIER_H_
#include <stdlib.h>

namespace IN {
namespace COMMON {

template <int MAX_PKT_SIZE >
struct SeqPacket
{
  char m_obj[MAX_PKT_SIZE];
  uint32_t m_size;
  uint64_t m_seq_num;
  uint64_t m_ts;
  IN::COMMON::MsgType m_type;

  void operator =(const SeqPacket& sp)
  {
    m_size = sp.m_size;
    m_seq_num = sp.m_seq_num;
    m_ts = sp.m_ts;
    m_type = sp.m_type;
    memcpy(m_obj,sp.m_obj,sp.m_size);
  }
};

class GapIdentifierIF
{
public:
  virtual ~GapIdentifierIF()
  {  }
  virtual void replayPacket(char *buf, uint64_t seq_num, IN::COMMON::MsgType mt) = 0;
  virtual void replayPacket(char *buf, uint64_t seq_num, uint64_t timestamp, IN::COMMON::MsgType mt)
  {
    LOG_E(__func__<<"not implemented");
  }
  virtual bool onPacketGap(uint64_t exp_start_seq_num,
                           uint64_t buff_seq_num, uint64_t incr_seq_num) = 0;
};

enum class Overflow{ MIN_SEQ, MAX_SEQ};
template <Overflow B = Overflow::MAX_SEQ, int MAX_PKT_SIZE = 512>
class GapIdentifier
{
private:
  PREVENT_COPY (GapIdentifier);
  void processBuffer()
  {
    int idx = get_index(m_read_index);
    LOG_I("Replaying sequence number: " << m_array[idx].m_seq_num);
    if (m_array[idx].m_ts == 0)
      {
        m_seq_cli->replayPacket(m_array[idx].m_obj, m_array[idx].m_seq_num, m_array[idx].m_type);
      }
    else
      {
        m_seq_cli->replayPacket(m_array[idx].m_obj, m_array[idx].m_seq_num, m_array[idx].m_ts, m_array[idx].m_type);
      }
    m_array[idx].m_seq_num = 0;

    ++m_read_index;
    ++m_expected_seq_num;
  }

protected:
  uint64_t m_expected_seq_num;
  uint32_t m_max_index;
  uint64_t m_max_seq_num;
  uint32_t m_write_index;
  uint32_t m_read_index;
  uint64_t m_packet_num;
  bool m_request_gap;

  SeqPacket<MAX_PKT_SIZE>* m_array;
  GapIdentifierIF* m_seq_cli;

  uint32_t get_index(uint32_t idx)
  {
    return idx % (m_max_index + 1);
  }

public:
  GapIdentifier(GapIdentifierIF* seq_cli, uint32_t size)
    : m_expected_seq_num(1)
    , m_max_index(0)
    , m_max_seq_num(0)
    , m_write_index(0)
    , m_read_index(0)
    , m_packet_num(0)
    , m_request_gap(true)
    , m_seq_cli(seq_cli)
  {
    uint32_t orig = size;

    if (size != orig)
      LOG_I("GapIdentifier buffer size is adjusted to be next power of 2 = " <<
            size << " from size = " << orig);
    else
      LOG_I("GapIdentifier buffer size = " << size);

    int ret = posix_memalign((void**) &m_array, 64, size * sizeof(SeqPacket<MAX_PKT_SIZE>));
    if (ret != 0)
      {
        LOG_E( "error in allocating memory using posix_memalign "<< strerror(ret));
        m_array = nullptr;
        return;
      }
    m_max_index = size - 1;
    memset(m_array, 0, size * sizeof(SeqPacket<MAX_PKT_SIZE>));
  }

  bool update(uint64_t seq_num, const char* obj, uint32_t size, uint64_t packet_num, IN::COMMON::MsgType mt, uint64_t timestamp = 0)
  {
    if (size > sizeof(SeqPacket<MAX_PKT_SIZE>) || size < 0)
      {
        LOG_E("Packet size exceeds buffer size : " << size);
        return false;
      }

    if (seq_num <= m_max_seq_num || seq_num < m_expected_seq_num)
      {
        LOG_D("Duplicate sequence number : "<< seq_num << " ignore packet!");
        return false;
      }

    if (seq_num > m_expected_seq_num )
      {
        LOG_D("m_max_index: " << m_max_index << " m_write_index: " <<
                  m_write_index << " m_read_index: " << m_read_index);
        if ((m_write_index - m_read_index) > m_max_index)
          {
            if (size > 0 && B == Overflow::MIN_SEQ)
              {
                int idx = get_index(m_write_index);
                LOG_D("Inserting packet in array at idx: "<< idx << " m_write_index: " <<
                          m_write_index  << " ReRequestFlag: " << m_request_gap);

                m_array[idx].m_seq_num = seq_num;
                memcpy(m_array[idx].m_obj, obj, size);
                m_array[idx].m_size = size;
                m_array[idx].m_ts = timestamp;
                m_array[idx].m_type = mt;
                ++m_write_index;
                ++m_read_index;
                m_max_seq_num = seq_num;
              }
            LOG_I("sequence number buffer filled, ignore/Overide packet!");
            // call to onPacketGap even buffer is filled
            if (m_request_gap && m_seq_cli->onPacketGap(m_expected_seq_num, m_array[get_index(m_read_index)].m_seq_num, seq_num))
              setReRequestFlag();
            
            return false;
          }

        if (size > 0)
          {
            int idx = get_index(m_write_index);
            LOG_D("Inserting packet in array at idx: "<< idx << " m_write_index: " <<
                      m_write_index  << " ReRequestFlag: " << m_request_gap);

            m_array[idx].m_seq_num = seq_num;
            memcpy(m_array[idx].m_obj, obj, size);
            m_array[idx].m_size = size;
            m_array[idx].m_ts = timestamp;
            m_array[idx].m_type = mt;
            ++m_write_index;
            m_max_seq_num = seq_num;
          }

        if (m_request_gap)
          {
            LOG_I("Read index seq number " << m_array[get_index(m_read_index)].m_seq_num << " packet_num: " << packet_num);

            bool success = false;
            if (m_array[get_index(m_read_index)].m_seq_num != 0)
              success = m_seq_cli->onPacketGap(m_expected_seq_num, m_array[get_index(m_read_index)].m_seq_num, seq_num);
            else
              success = m_seq_cli->onPacketGap(m_expected_seq_num, seq_num, seq_num);
            if (success)
              setReRequestFlag();
          }
      }
    else
      {
        m_packet_num = packet_num;
      }
    return true;
  }

  uint64_t numElements() const
  {
    return m_write_index - m_read_index;
  }

  bool isInSeq(uint64_t seq_num) const 
  {
    return m_expected_seq_num == seq_num;
  }

  bool isDuplicate(uint64_t seq_num) const 
  {
    return  m_expected_seq_num > seq_num;
  }

  void updateSeqNum(uint64_t seq_num)
  {
    m_expected_seq_num = seq_num + 1;
  }

  void updateSeqNumFlush(uint64_t seq_num)
  {
    m_expected_seq_num = seq_num + 1;
    if (m_read_index < m_write_index)
    {
      LOG_D("Draining/Replaying buffer");
      drainAndReplay();
    }
  }

  void drainPacket()
  {
    while( m_read_index < m_write_index )
      {
        LOG_D("Draining sequence number: " << m_array[get_index(m_read_index)].m_seq_num);
        m_array[get_index(m_read_index)].m_seq_num = 0;
        ++m_read_index;
      }
  }

  void drainAndReplay()
  {
    while( m_read_index < m_write_index )
      {
        LOG_D("Draining/Replaying sequence number: " << m_array[get_index(m_read_index)].m_seq_num  <<
                  " Expected seq_num: " << m_expected_seq_num);

        if (m_array[get_index(m_read_index)].m_seq_num <  m_expected_seq_num)
        {
          m_array[get_index(m_read_index)].m_seq_num = 0;
          ++m_read_index;
        }
        else if (m_array[get_index(m_read_index)].m_seq_num ==  m_expected_seq_num)
        {
          processBuffer();
        }
        else
        {
          break;
        }
      }

  }

  void drainPacket(uint64_t seq_num)
  {
    while( m_read_index < m_write_index &&
           m_array[get_index(m_read_index)].m_seq_num <=  seq_num )
      {
        LOG_D("Draining sequence number: " << m_array[get_index(m_read_index)].m_seq_num);
        m_array[get_index(m_read_index)].m_seq_num = 0;
        ++m_read_index;
      }
    LOG_D("readIdx: " << m_read_index << ", writeIDx: " << m_write_index << 
          " sequence number: " << m_array[get_index(m_read_index)].m_seq_num
          << " expected_seq: " << m_expected_seq_num );
    
  }

  void replayPacket(uint64_t seq_num)
  {
    m_expected_seq_num = seq_num;
    unsetReRequestFlag();

    while(m_read_index < m_write_index &&
          m_array[get_index(m_read_index)].m_seq_num ==  m_expected_seq_num )
      {
        processBuffer();
      }

    if (m_read_index < m_write_index)
      {
        LOG_D("Read write index are not equal m_read_index: " << m_read_index
                  << " m_write_index: " << m_write_index);

        if (m_seq_cli->onPacketGap(m_expected_seq_num,
                                   m_array[get_index(m_read_index)].m_seq_num,
                                   m_array[get_index(m_read_index)].m_seq_num))
          setReRequestFlag();
      }
    else
      {
        LOG_D("Read write index are equal m_read_index: " << m_read_index
                  << " m_write_index: " << m_write_index);
        LOG_D("m_expected_seq_num: " << m_expected_seq_num << " start seq_num: " << seq_num);
      }
  }

  bool getReRequestFlag()
  {
    return m_request_gap;
  }

  void setReRequestFlag()
  {
    m_request_gap = false;
  }

  void unsetReRequestFlag()
  {
    m_request_gap = true;
  }
};

}}
#endif 
