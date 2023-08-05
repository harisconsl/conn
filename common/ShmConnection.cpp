#include "ShmConnection.h"
#include "Logger.h"
using namespace IN::COMMON;

#define FILE_SIZE 1024

ShmConnection::ShmConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , stream_(io_context)
  , m_fd(-1)
  , mapped_memory(nullptr)
{ }

ShmConnection* ShmConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  if (!host.size())
    {
      LOG_I("Invalid address for shm : " << host);
      return nullptr;
    }

  ShmConnection* connection = new ShmConnection(true, io_context);
  connection->m_address = host;
  return connection;
}

int ShmConnection::get_fd()
{
  LOG_I("ShmConnection::file descriptor");
  return m_fd;
}

int ShmConnection::open()
{
  m_fd = ::open(m_address.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (m_fd < 0) {
    LOG_E("Failed to open file" << m_address);
    return -1;
  }

  // Set file size
  ::ftruncate(m_fd, FILE_SIZE);

  // Memory map the file
  mapped_memory = ::mmap(nullptr, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
  if (mapped_memory == MAP_FAILED) {
    LOG_E( "Failed to map memory");
    m_fd = -1;
    return m_fd;
  }

  //  boost::asio::mutable_buffer buffer(mapped_memory, FILE_SIZE);
  // Associate the file descriptor with the boost::asio::posix::stream_descriptor
  stream_.assign(m_fd);
  //boost::asio::async_write(stream_, buffer, std::bind(&ShmConnection::handle_write, this, std::placeholders::_1, std::placeholders::_2));
  auto p = m_read_buf.write_buffer();
  stream_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(p.first),p.second),
                          std::bind(&ShmConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));
}


void ShmConnection::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  char buffer[1024];
  if (!error)
    {
      LOG_D("Message sent!");
      // stream_->async_read_some(boost::asio::buffer(buffer), std::bind(&ShmConnection::handle_read, this,
      //   							      std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      LOG_E("Write error: " << error.message());
    }
}

void ShmConnection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  LOG_I("Message recieved! : " << bytes_transferred);
  if (!error)
    {
      LOG_I("Received message: " << std::string(reinterpret_cast<const char*>(m_read_buf.read_buffer().first),
                                                bytes_transferred) <<"\n");
      m_read_buf.advance_write(bytes_transferred);
      LOG_I("address: " << static_cast<void*>(m_read_buf.read_buffer().first));
      
      // char buf[128];
      // unsigned char* msg_buf = reinterpret_cast<unsigned char*>(m_read_buf.read_buffer().first);
      // for (uint16_t i = 0; i < bytes_transferred; ++i)
      //   {
      //     sprintf(&buf[2*i], "%02x", *(msg_buf+i));
      //   }
      // LOG_I("Recieved msg => " << buf );
      m_read_buf.advance_read(bytes_transferred);
      async_read();
    }
  else
    {
      LOG_E("Read error: " << error.message());
    }
}

void ShmConnection::do_close()
{

}

bool ShmConnection::is_open()
{
  if (m_fd < 0)
    return false;
  return true;
}

std::string ShmConnection::address()
{
  return m_address;
}

void ShmConnection::async_read()
{
  LOG_I("address: " << static_cast<void*>(m_read_buf.write_buffer().first));
  auto p = m_read_buf.write_buffer();
  stream_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(p.first),p.second),
                          std::bind(&ShmConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));

}
