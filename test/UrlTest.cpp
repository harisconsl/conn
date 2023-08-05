#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include "cstdio"

#include "../common/Url.h"

using ::testing::_;

using namespace IN::COMMON;
// Test Fixture for GapIdentifier Class
struct Url_F : public testing::Test
{
  Url* m_tcpUrl;
  Url* m_unixUrl;
  Url* m_mcpUrl;
  Url* m_shmUrl;

  void SetUp()
  {
    m_tcpUrl = new Url("tcp://0.0.0.0:40881,rbuf=1024,rmax=8196");
    m_unixUrl = new Url("unix://tmp/mysocket,rbuf=128,rmax=16382");
    m_mcpUrl = new Url("mcp://233.24.10.1:19601");
    m_shmUrl = new Url("shm://agg_book,size=1024");

  }
  void TearDown()
  {
    delete m_tcpUrl;
    delete m_unixUrl;
    delete m_mcpUrl;
    delete m_shmUrl;
  }
};

TEST_F(Url_F, TcpTest)
{
  EXPECT_EQ(m_tcpUrl->get_address(), "0.0.0.0");
  EXPECT_EQ(m_tcpUrl->get_option("port"), "40881");
  EXPECT_EQ(m_tcpUrl->get_option("rbuf"), "1024");
  EXPECT_EQ(m_tcpUrl->get_option("rmax"), "8196");
}

TEST_F(Url_F, McpTest)
{
  EXPECT_EQ(m_mcpUrl->get_address(), "233.24.10.1");
  EXPECT_EQ(m_mcpUrl->get_option("port"), "19601");
}

TEST_F(Url_F, UnixTest)
{
  EXPECT_EQ(m_unixUrl->get_address(), "tmp/mysocket");
  EXPECT_EQ(m_unixUrl->get_option("rbuf"), "128");
  EXPECT_EQ(m_unixUrl->get_option("rmax"), "16382");
}

TEST_F(Url_F, ShmTest)
{
  EXPECT_EQ(m_shmUrl->get_address(), "agg_book");
  EXPECT_EQ(m_shmUrl->get_option("size"), "1024");
}
