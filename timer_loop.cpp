#include <boost/asio.hpp>
#include <iostream>

int factorial(int n)
{
  int res = 1;
  for (int i = 1; i <= n ; ++i)
    res = res * i;

  return res;
}

void handleEvent(const boost::system::error_code& error) {
  if (!error)
    {
      // Perform the desired task or operation
      
      // Reschedule the event
      // For example, reschedule it after 1 second
      std::cout << "I am calculating factorial" << std::endl;
      factorial(10);
      boost::asio::steady_timer timer(io_context);
      timer.expires_after(std::chrono::seconds(1));
      timer.async_wait(handleEvent);
    }
}

int main() {
    boost::asio::io_context io_context;

    // Schedule the first event
    boost::asio::steady_timer timer(io_context);
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait(handleEvent);

    // Run the event loop
    io_context.run();

    return 0;
}
