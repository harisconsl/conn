
#define BOOST_ASIO_ENABLE_HANDLER_TRACKING 1

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
//#include <boost_asio/example/cpp11/handler_tracking/custom_tracking.hpp>
#include <iostream>
#include <chrono>


boost::asio::io_context io_context;
int factorial(int n)
{
  int res = 1;
  for (int i = 1; i <= n ; ++i)
    res = res * i;

  return res;
}

void handleEvent(const boost::system::error_code& error)
{
  if (!error)
    {
      // Perform the desired task or operation
      
      // Reschedule the event
      // For example, reschedule it after 1 second
      std::cout << "I am calculating factorial : " << factorial(10) <<std::endl;
 
      boost::asio::steady_timer timer(io_context);
      timer.expires_after(std::chrono::seconds(10));
      timer.async_wait(handleEvent);
    }
  else
    std::cout << "Boost::error : " << error.message() <<std::endl;  
}

int main()
{
  // Schedule the first event
  boost::asio::steady_timer timer(io_context);
  timer.expires_after(std::chrono::seconds(1));

  std::function<void(const boost::system::error_code&)> handler;
  
  handler = [&](const boost::system::error_code& ec) {
    if (!ec)
      {
	std::cout << "factorial is " << factorial(10) << std::endl;
	timer.expires_after(std::chrono::seconds(1));
	timer.async_wait(handler);
      }
    else
      std::cout << "Boost::error " << ec.message() << std::endl;
  };
  timer.async_wait(handler);
  
  // Run the event loop
  io_context.run();
  return 0;
}
