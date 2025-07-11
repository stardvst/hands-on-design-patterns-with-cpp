#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <latch>

static constexpr size_t N = 1024;

struct Data
{
  int n;
  double x;
};

Data data[N]{};
std::atomic<size_t> size(0);

void produce(int n, int delay, std::latch &latch)
{
  std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay % 100)); // Just to make threads run in order other than 0, 1, 2...
  const int iter_count = 1 << (12 + n);

  const size_t s = size.fetch_add(1, std::memory_order_acq_rel);
  data[s].n = iter_count;
  double &x = data[s].x;
  x = 0;

  for (int i = 0, d = 1; i != iter_count; ++i, d += 4)
    x += (4.0 / d - 4.0 / (d + 2));

  // One more thread is done.
  latch.count_down();
}

int main()
{
  constexpr size_t nthread = 5;
  std::jthread t[nthread];

  std::latch latch(nthread);
  for (size_t i = 0; i != nthread; ++i)
    t[i] = std::jthread(produce, i, rand(), std::ref(latch));

  // Wait for producers to finish.
  latch.wait();

  // Print all data we have produced.
  for (size_t i = 0, n = size.load(std::memory_order_relaxed); i != n; ++i)
    std::cout << data[i].x << "(" << data[i].n << ") ";

  std::cout << std::endl;
}
