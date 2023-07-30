#include "ShmConnection.h"
#include "Logger.h"
using namespace IN::COMMON;

#define FILE_SIZE 1024

ShmConnection::ShmConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , stream_(io_context)
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


int ShmConnection::open()
{
  int fileDescriptor = ::open(m_address.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fileDescriptor < 0) {
    LOG_E("Failed to open file" << m_address);
    return -1;
  }

  // Set file size
  ::ftruncate(fileDescriptor, FILE_SIZE);

  // Memory map the file
  mapped_memory = ::mmap(nullptr, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
  if (mapped_memory == MAP_FAILED) {
    LOG_E( "Failed to map memory");
    return 1;
  }

  boost::asio::mutable_buffer buffer(mapped_memory, FILE_SIZE);
  // Associate the file descriptor with the boost::asio::posix::stream_descriptor
  stream_.assign(fileDescriptor);
  boost::asio::async_write(stream_, buffer, std::bind(&ShmConnection::handle_write, this, std::placeholders::_1, std::placeholders::_2));
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
  // if (!error)
  //   {
  //     std::cout << "Received message: " << std::string(buffer.data(), bytes_transferred) << std::endl;
  //   }
  // else
  //   {
  //     std::cout << "Read error: " << error.message() << std::endl;
  //   }
}

void ShmConnection::do_close()
{
}

bool ShmConnection::is_open()
{
  return false;
}

std::string ShmConnection::address()
{
  return m_address;
}

