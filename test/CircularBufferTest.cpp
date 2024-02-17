#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include "cstdio"

#include "../common/CircularBuffer.h"
#include "../engine/book/L2SnapshotLite.h"

using ::testing::_;

using namespace IN::COMMON;
using namespace IN::ENGINE;
// Test Fixture for GapIdentifier Class
struct CircularBuffer_F : public testing::Test
{
  CircularBuffer<SeqPacket<IN::ENGINE::L2SnapshotLite>, 4> m_buf;
  void SetUp()
  {  }

  void TearDown()
  {
    
  }
};

TEST_F(CircularBuffer_F, TcpTest)
{
    uint64_t ts1 = 1706695492309000000;
    std::unique_ptr<IN::ENGINE::L2SnapshotLite> l1(new IN::ENGINE::L2SnapshotLite);
    SeqPacket<IN::ENGINE::L2SnapshotLite> s1(ts1,l1);
    m_buf.update(s1);
    EXPECT_EQ(1,m_buf.write_index());

    uint64_t ts2 = 1706695492319000000;
    IN::ENGINE::L2SnapshotLite* l2 = new IN::ENGINE::L2SnapshotLite;
    SeqPacket<IN::ENGINE::L2SnapshotLite> s2(ts2,l2);
    m_buf.update(s2);
    EXPECT_EQ(2,m_buf.write_index());

    uint64_t ts3 = 1706695492329000000;
    IN::ENGINE::L2SnapshotLite* l3 = new IN::ENGINE::L2SnapshotLite;
    SeqPacket<IN::ENGINE::L2SnapshotLite> s3(ts3,l3);
    m_buf.update(s3);
    EXPECT_EQ(3,m_buf.write_index());
    
    uint64_t ts4 = 1706695492329000000;
    IN::ENGINE::L2SnapshotLite* l4 = new IN::ENGINE::L2SnapshotLite;
    SeqPacket<IN::ENGINE::L2SnapshotLite> s4(ts4,l4);
    m_buf.update(s4);
    EXPECT_EQ(0,m_buf.write_index());
    
    uint64_t ts5 = 1706695492329000000;
    IN::ENGINE::L2SnapshotLite* l5 = new IN::ENGINE::L2SnapshotLite;
    SeqPacket<IN::ENGINE::L2SnapshotLite> s5(ts5,l5);
    m_buf.update(s5);
    EXPECT_EQ(1,m_buf.write_index());

    EXPECT_TRUE(m_buf.find(1706695492329000000).has_value());
    EXPECT_TRUE(true);
}
