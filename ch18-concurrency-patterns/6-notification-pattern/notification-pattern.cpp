#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

struct Data
{
  int n{};
  double x{};
};

std::mutex mtx;
std::condition_variable cv;
Data data{};

void producer()
{
  {
    std::cout << "Producer thread is running..." << std::endl;
    std::lock_guard l(mtx);

    data.n = 1 << 30;
    data.x = 0;

    for (int i = 0, d = 1; i != data.n; ++i, d += 4)
      data.x += (4.0 / d - 4.0 / (d + 2));
  }

  std::cout << "Producer thread is notifying the main thread..." << std::endl;
  cv.notify_one();
}

int main()
{
  std::cout << "Main thread is waiting for the count to reach 10..." << std::endl;

  std::unique_lock<std::mutex> lock(mtx);
  std::thread producer_thread(producer);

  cv.wait(lock, []
          { return data.n != 0; });

  std::cout << data.n << " " << data.x << std::endl;

  producer_thread.join();
  std::cout << "Producer thread has finished." << std::endl;
  return 0;
}
