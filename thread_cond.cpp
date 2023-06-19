#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable cv;
bool isReady = false;
int i = 0;
void producerFunc() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    {
      std::lock_guard<std::mutex> lock(mutex);
      isReady = true;
    }
    cv.notify_one();
}

void consumerFunc() {
  std::unique_lock<std::mutex> lock(mutex);
  //  cv.wait(lock, []{return i == 1;});
  cv.wait(lock, []{return isReady;});
  std::cout << "Consumer: Work can now be performed." << std::endl;
}

int main() {
  std::thread producerThread(producerFunc);
  std::thread consumerThread(consumerFunc);
  
  producerThread.join();
  consumerThread.join();
  
  return 0;
}
