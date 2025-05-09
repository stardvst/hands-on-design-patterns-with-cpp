#include <iostream>

class Timer
{
public:
  template <typename F>
  explicit Timer(const std::string &label, F &&f, std::size_t N) : m_startTime(std::chrono::high_resolution_clock::now()), m_label(label)
  {
    for (std::size_t i = 0; i < N; ++i)
    {
      f();
    }
  }

  double elapsed() const
  {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedTime = endTime - m_startTime;
    return elapsedTime.count();
  }

  ~Timer()
  {
    std::cout << m_label << " elapsed time: " << elapsed() << " ms" << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
  std::string m_label;
};

template <typename T, typename Deleter>
class smart_ptr
{
public:
  smart_ptr(T *ptr, Deleter deleter) : m_ptr(ptr), m_deleter(deleter) {}
  ~smart_ptr() { m_deleter(m_ptr); }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

private:
  T *m_ptr;
  Deleter m_deleter;
};

template <typename T>
class smart_ptr_te
{
  struct deleter_base
  {
    virtual void apply(void *ptr) = 0;
    virtual ~deleter_base() = default;
  };

  template <typename Deleter>
  struct deleter_impl : deleter_base
  {
    deleter_impl(Deleter deleter) : m_deleter(deleter) {}
    void apply(void *ptr) override { m_deleter(static_cast<T *>(ptr)); }
    Deleter m_deleter;
  };

public:
  template <typename Deleter>
  smart_ptr_te(T *ptr, Deleter deleter) : m_ptr(ptr), m_deleter(new deleter_impl<Deleter>(deleter)) {}

  ~smart_ptr_te()
  {
    if (m_deleter)
      m_deleter->apply(m_ptr);
    delete m_deleter;
  }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

private:
  T *m_ptr;
  deleter_base *m_deleter;
};

struct simple_deleter
{
  template <typename T>
  void operator()(T *ptr) { delete ptr; }
};

int main()
{
  constexpr std::size_t N = 1000000;
  double smartPtrTime = 0.0, smartPtrTeTime = 0.0;

  simple_deleter deleter;
  {
    Timer timer("smart_ptr", [&]()
                { smart_ptr<int, simple_deleter> sptr(new int(42), deleter); (void)sptr; }, N);
    smartPtrTime = timer.elapsed();
  }

  {
    Timer timer("smart_ptr_te", [&]()
                { smart_ptr_te<int> sptr(new int(42), deleter); (void)sptr; }, N);
    smartPtrTeTime = timer.elapsed();
  }

  if (smartPtrTime < smartPtrTeTime)
  {
    std::cout << "smart_ptr is faster than smart_ptr_te" << std::endl;
  }
  else
  {
    std::cout << "smart_ptr_te is faster than smart_ptr" << std::endl;
  }
}
