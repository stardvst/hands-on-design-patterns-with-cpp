#include <iostream>
#include <chrono>
#include <memory>

class Timer
{
public:
  explicit Timer(const std::string &label) : m_startTime(std::chrono::high_resolution_clock::now()), m_label(label) {}

  ~Timer()
  {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedTime = endTime - m_startTime;
    std::cout << m_label << " elapsed time: " << elapsedTime.count() << " ms" << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
  std::string m_label;
};

class A
{
public:
  void f(int i) { m_i += i; }

protected:
  int m_i{};
};

template <typename D>
class B
{
public:
  void f(int i) { static_cast<D *>(this)->f(i); }

protected:
  int m_i{};
};

class D : public B<D>
{
public:
  void f(int i) { m_i += i; }
};

int main()
{
  {
    Timer timer("non-virtual");
    int i = 0;
    auto a = std::make_unique<A>();
    for (int j = 0; j < 1000000; ++j)
      a->f(++i);
  }

  {
    Timer timer("crtp");
    int i = 0;
    std::unique_ptr<B<D>> b = std::make_unique<D>();
    for (int j = 0; j < 1000000; ++j)
      b->f(++i);
  }

  return 0;
}
