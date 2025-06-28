#include <iostream>
#include <atomic>
#include <thread>

struct Data
{
  int n{};
  double x{};
};

std::atomic_flag flag;
Data data;

void producer()
{
  {
    std::cout << "Producer thread is running..." << std::endl;

    data.n = 1 << 30;
    data.x = 0;

    for (int i = 0, d = 1; i != data.n; ++i, d += 4)
      data.x += (4.0 / d - 4.0 / (d + 2));
  }

  std::cout << "Producer thread is notifying the main thread..." << std::endl;
  flag.test_and_set(std::memory_order_release);
}

int main()
{
  std::cout << "Main thread is waiting for the count to reach 10..." << std::endl;

  flag.clear();
  std::thread producer_thread(producer);

  while (!flag.test(std::memory_order_acquire))
    ;

  std::cout << data.n << " " << data.x << std::endl;

  producer_thread.join();
  std::cout << "Producer thread has finished." << std::endl;
  return 0;
}
