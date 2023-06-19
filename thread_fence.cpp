#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> data ;

void helper1()
{
    // Perform some operations on data
    // ...

    // Enforce memory synchronization
  data = 10;
  std::cout << "thread1 : " << ++data << std::endl;
  std::atomic_thread_fence(std::memory_order_release);
}

void helper2()
{
    // Enforce memory synchronization
    std::atomic_thread_fence(std::memory_order_acquire);
    data = 20;
    std::cout << "thread2 : " << ++data << std::endl;
    // Perform operations on data
    // ...
}

int main()
{
    std::thread t1(helper1);
    std::thread t2(helper2);

    t1.join();
    t2.join();

    return 0;
}
