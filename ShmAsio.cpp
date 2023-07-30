#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

namespace asio = boost::asio;
namespace posix = asio::posix;

constexpr std::size_t FILE_SIZE = 4096;

int main() {
    try {
        asio::io_context ioContext;
        posix::stream_descriptor stream(ioContext);
        int fileDescriptor = ::open("file.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fileDescriptor < 0) {
            std::cerr << "Failed to open file" << std::endl;
            return 1;
        }

        // Set file size
        ::ftruncate(fileDescriptor, FILE_SIZE);

        // Memory map the file
        void* mappedMemory = ::mmap(nullptr, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
        if (mappedMemory == MAP_FAILED) {
            std::cerr << "Failed to map memory" << std::endl;
            return 1;
        }

        // Construct a boost::asio::mutable_buffer from the mapped memory
        asio::mutable_buffer buffer(mappedMemory, FILE_SIZE);

        // Associate the file descriptor with the boost::asio::posix::stream_descriptor
        stream.assign(fileDescriptor);

        // Perform I/O operations using Boost.Asio
        asio::async_write(stream, buffer,
            [mappedMemory](const boost::system::error_code& error, std::size_t /*bytesTransferred*/) {
                if (!error) {
                    std::cout << "Write operation completed successfully" << std::endl;
                } else {
                    std::cerr << "Write error: " << error.message() << std::endl;
                }

                // Unmap the memory when done
                ::munmap(mappedMemory, FILE_SIZE);
            });

        ioContext.run();

        // Close the file descriptor
        ::close(fileDescriptor);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
