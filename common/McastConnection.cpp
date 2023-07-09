using namespace std;
using namespace IN::COMMON;

typedef boost::asio::ip bai;

McastConnection::McastConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context)
{ }

McastConnection* McastConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  std::string port = url.get_option("port");

  if (!host.size() || !port.size())
    {
      LOG_I("Invalid host and/or port");
      return nullptr;
    }

  McastConnection connection = new McastConnection(is_stream, io_context);

  // intialize the private memeberiof the class
  connection->m_host = host;
  connection->m_port = port;
}

void McastConnection::join_group()
{
  socket_(ioContext, bai::udp::endpoint(bai::udp::v4(), 0)
  bai::address multicastAddr = bai::address::from_string(m_host);
  socket_.set_option(bai::multicast::join_group(m_host));
  socket_.set_option(bai::multicast::outbound_interface(interfaceAddr));
}

void McastConnection::leave_group()
{
  bai::address multicastAddr = socket_.local_endpoint().address();
  socket_.set_option(bai::multicast::leave_group(multicastAddr));
}

bool McastConnection::handle_recv(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
  udp::endpoint endpointp(bai::address::from_string(m_host, std::stoi(m_port));
			  socket_.async_receive_from( boost::asio::buffer(buffer), endpoint,
						      std::bind(&McastConnection::handle_read, this, 
								std::placeholders::_1, std::placeholders::_2));
}

void UnixConnection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
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


