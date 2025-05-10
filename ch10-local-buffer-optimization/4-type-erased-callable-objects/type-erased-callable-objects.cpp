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

template <typename T>
class smart_ptr_te_lb
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
  smart_ptr_te_lb(T *ptr, Deleter deleter)
      : m_ptr(ptr), m_deleter(
                        (sizeof(Deleter) > sizeof(m_buffer))
                            ? new deleter_impl<Deleter>(deleter)
                            : new(m_buffer) deleter_impl<Deleter>(deleter)) {}

  ~smart_ptr_te_lb()
  {
    if (m_deleter)
      m_deleter->apply(m_ptr);

    if (static_cast<void *>(m_deleter) != static_cast<void *>(m_buffer))
    {
      // If the deleter was allocated on the heap, delete it
      delete m_deleter;
    }
    else
    {
      // If the deleter was allocated on the stack, do not delete it, just call the destructor
      // This is safe because the deleter is a POD type
      // and its destructor does not need to be called explicitly.
      m_deleter->~deleter_base();
    }
  }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

private:
  T *m_ptr;
  deleter_base *m_deleter;
  char m_buffer[16]; // Local buffer for small objects
};

struct simple_deleter
{
  template <typename T>
  void operator()(T *ptr) { delete ptr; }
};

int main()
{
  constexpr std::size_t N = 1000000;
  double smartPtrTime = 0.0, smartPtrTeTime = 0.0, smartPtrTeLbTime = 0.0;

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

  {
    Timer timer("smart_ptr_te_lb", [&]()
                { smart_ptr_te_lb<int> sptr(new int(42), deleter); (void)sptr; }, N);
    smartPtrTeLbTime = timer.elapsed();
  }

  std::cout << "The fastest smart pointer is: " << (smartPtrTime < smartPtrTeTime ? "smart_ptr" : "smart_ptr_te") << std::endl;
  std::cout << "The fastest type-erased smart pointer is: " << (smartPtrTeTime < smartPtrTeLbTime ? "smart_ptr_te" : "smart_ptr_te_lb") << std::endl;}
