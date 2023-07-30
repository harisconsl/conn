/*
#include<iostream>

static const unsigned short kDefaultPort = 33333;

static void runServer()
{
    setAsNonBlocking(server);
    int ws = buildInitialWatchSet(server);
}

static const int kMaxEvents = 64;

static int buildInitialWatchSet(int server)
{
    int ws = epoll_create1(0);
    struct epoll_event info = {.events = EPOLLIN | EPOLLET, .data = {.fd = server}};
    epoll_ctl(ws, EPOLL_CTL_ADD, server, &info);
    return ws;
}

int main(int argc, char** argv)
{
    int server = createServerSocket(kDefaultPort);
    runServer(server);

    return 0;
}*/

#include <iostream>
#include <boost/asio.hpp>
#include <sys/epoll.h>

int main()
{
    // Create an epoll file descriptor
    int epollFd = epoll_create1(0);
    if (epollFd == -1)
    {
        std::cerr << "Failed to create epoll file descriptor" << std::endl;
        return 1;
    }

    // Create a socket and bind it to a local address
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket socket(io);
    socket.open(boost::asio::ip::tcp::v4());
    socket.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));

    // Add the socket to the epoll file descriptor
    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = socket.native_handle();
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socket.native_handle(), &event) == -1)
    {
        std::cerr << "Failed to add socket to epoll" << std::endl;
        return 1;
    }

    // Wait for events using epoll
    epoll_event events[10];
    while (true)
    {
        int numEvents = epoll_wait(epollFd, events, 10, -1);
        if (numEvents == -1)
        {
            std::cerr << "epoll_wait error" << std::endl;
            return 1;
        }

        for (int i = 0; i < numEvents; ++i)
        {
            if (events[i].events & EPOLLIN)
            {
                if (events[i].data.fd == socket.native_handle())
                {
                    // Handle incoming data from the socket
                    boost::asio::ip::tcp::endpoint senderEndpoint;
                    boost::array<char, 128> buffer;
                    boost::system::error_code error;
                    std::size_t bytesRead = socket.receive_from(boost::asio::buffer(buffer), senderEndpoint, 0, error);
                    if (!error)
                    {
                        std::cout << "Received: " << std::string(buffer.data(), bytesRead) << std::endl;
                    }
                }
            }
        }
    }

    // Close the epoll file descriptor
    close(epollFd);

    return 0;
}
