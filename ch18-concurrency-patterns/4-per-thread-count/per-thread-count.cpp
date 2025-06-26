#include <iostream>
#include <thread>

void events(unsigned int value, std::size_t &counter)
{
  for (std::size_t i = 0; i < 100; ++i)
  {
    if (rand_r(&value) % 10 == 0)
      ++counter;
  }
}

int main()
{
  alignas(64) std::size_t n1 = 0;
  alignas(64) std::size_t n2 = 0;

  std::thread t1(events, 1, std::ref(n1));
  std::thread t2(events, 2, std::ref(n2));
  t1.join();
  t2.join();

  std::size_t total = n1 + n2;
  std::cout << total << std::endl;
}
