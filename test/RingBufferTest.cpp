#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include "cstdio"

#include "../common/RingBuffer.h"

using ::testing::_;

using namespace IN::COMMON;
// Test Fixture for GapIdentifier Class
struct RingBuffer_F : public testing::Test
{
  RingBuffer m_read_buf;

  void SetUp()
  {
    m_read_buf.alloc(1024*1024);
  }
  void TearDown()
  {
  }
};

TEST_F(RingBuffer_F, TcpTest)
{
  char arr[11] ="1234567890";

  memcpy( m_read_buf.write_buffer().first, arr, 10);
  m_read_buf.advance_write(10);
  std::string str(reinterpret_cast<const char*>(m_read_buf.read_buffer().first), m_read_buf.read_buffer().second);
  m_read_buf.advance_read(10);

  EXPECT_EQ(str, std::string("1234567890"));

  char name[13]="piyush gupta";
  memcpy( m_read_buf.write_buffer().first, name, 12);
  m_read_buf.advance_write(12);
  std::string str1(reinterpret_cast<const char*>(m_read_buf.read_buffer().first), m_read_buf.read_buffer().second);
  m_read_buf.advance_read(12);

  EXPECT_EQ(str1, std::string("piyush gupta"));
}
