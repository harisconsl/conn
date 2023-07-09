
ShmConnection::ShmConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , stream_(io_context)
  , mapped_memory(nullptr)
{ }

ShmConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  if (!host.size())
    {
      LOG_I("Invalid address for unix socket");
      return nullptr;
    }

  ShmConnection* connection = new ShmConnection(is_stream, io_context);
  connection->m_host = host;
  return Connection;
}


ShmConnection::open()
{
  //  if (socket_.is_open())
  int fileDescriptor = ::open("file.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fileDescriptor < 0) {
    std::cerr << "Failed to open file" << std::endl;
    return 1;
  }

  // Set file size
  ::ftruncate(fileDescriptor, FILE_SIZE);

  // Memory map the file
  mapped_memory = ::mmap(nullptr, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
  if (mapped_memory == MAP_FAILED) {
    std::cerr << "Failed to map memory" << std::endl;
    return 1;
  }

  asio::mutable_buffer buffer(mappedMemory, FILE_SIZE);

  // Associate the file descriptor with the boost::asio::posix::stream_descriptor
  stream.assign(fileDescriptor);
  asio::async_write(stream, buffer,, std::bind(&ShmConnection::handle_write, this, std::placeholders::_1, std::placeholders::_2);
}

void ShmConnection::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred)
{


  if (!error)
    {
      std::cout << "Message sent!" << std::endl;
      socket_->async_read_some(boost::asio::buffer(buffer), std::bind(&UnixConnection::handle_read, this,
								      std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      std::cout << "Write error: " << error.message() << std::endl;
    }
}

void ShmConnection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  if (!error)
    {
      std::cout << "Received message: " << std::string(buffer.data(), bytes_transferred) << std::endl;
    }
  else
    {
      std::cout << "Read error: " << error.message() << std::endl;
    }
}

void ShmConnection::do_close()
{
  socket_->cancel(); // Cancel any pending asynchronous operations
  // 3. Close the socket
  boost::system::error_code ec;
  //  socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  if (ec) {
    // Handle error
    std::cout << "Socket close error 1: " << ec.message() << std::endl;
  }
  socket_->close(ec);
  if (ec)
    {
      // Handle error
      std::cout << "Socket close error 2: " << ec.message() << std::endl;
    }
}

bool ShmConnection::is_open()
{
  if (socket_.is_open())
    return true;
  return false;
}

std::string ShmConnection::address()
{
  return m_address;
}

