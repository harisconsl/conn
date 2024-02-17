#ifndef _SEQUENCER_H_
#define _SEQUNECER_H_

#include <vector>

namespace IN {
namespace COMMON {

struct PacketGap
{
  uint32_t start_gap_packet_num;
  uint32_t end_gap_packet_num;
};

enum class FeedType { INCREMENTAL, REPEAT};

class Sequencer
{
private:
  PREVENT_COPY (Sequencer);

protected:
  uint64_t m_expected_packet_num;
  uint32_t m_channel_id;
  FeedType m_mode;
  std::vector<PacketGap> m_packet_gaps;
  std::function<void(int, uint64_t)> m_callback;
public:
  Sequencer(uint32_t channel_id, FeedType mode, std::function<void(int, uint64_t)> callback )
    : m_expected_packet_num (1)
    , m_channel_id(channel_id)
    , m_mode(mode)
    , m_callback(callback)
  { }

  bool isDuplicate(uint64_t packet_num)
  {
    if (m_expected_packet_num > packet_num)
      return true;
    return false;
  }

  bool isInSeq(uint64_t packet_num, bool callback_needed)
  {
    LOG_DEBUG("Expected packet num: "<< m_expected_packet_num << " packet num: " << packet_num);
    if (m_mode == FeedType::REPEAT && m_expected_packet_num != packet_num )
      return false;

    if (m_expected_packet_num > packet_num)
      return false;
    
    if ( m_expected_packet_num != packet_num && callback_needed)
      {
        LOG_INFO("PacketGap found. Expected packet num: "<< m_expected_packet_num << " packet num: " << packet_num);
        PacketGap gap;
        gap.start_gap_packet_num = m_expected_packet_num;
        gap.end_gap_packet_num = packet_num - 1;
        m_packet_gaps.push_back(gap);
        m_callback(m_channel_id, m_expected_packet_num);
        return false;
      }

    m_expected_packet_num = packet_num + 1;
    return true;
  }

  void resetSeqNo()
  {
    m_expected_packet_num = 1;
  }

  void resetSeqNo(uint64_t seq_num) 
  {
    m_expected_packet_num = seq_num;
  }

};

}}

#endif 
