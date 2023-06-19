#include <iostream>
#include <thread>
#include <sys/eventfd.h>
#include <unistd.h>

void threadFunc(int id, int& counter, int event_fd) {
    // Do some work
    
    // Increment the counter
    counter++;
    
    // Signal the eventfd to indicate completion
    uint64_t signal = 1;
    write(event_fd, &signal, sizeof(signal));
}

int main() {
    int num_threads = 5;
    int counter = 0;
    int event_fd = eventfd(0, EFD_NONBLOCK);

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(threadFunc, i, std::ref(counter), event_fd);
    }

    // Wait for all threads to complete
    for (int i = 0; i < num_threads; ++i) {
        // Wait for the eventfd to be signaled
        uint64_t signal;
        read(event_fd, &signal, sizeof(signal));
    }

    // All threads have completed
    std::cout << "Counter: " << counter << std::endl;

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
