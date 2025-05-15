#include <cstdlib> // For rand
#include <iostream>
#include <stdexcept>
#include <functional>

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

class ScopeGuard
{
public:
  template <typename Func>
  ScopeGuard(Func &&func)
      : m_func(std::move(func))
  {
  }

  ScopeGuard &operator=(ScopeGuard const &) = delete;

  ScopeGuard(ScopeGuard &&other)
      : m_commit(other.m_commit), m_func(std::move(other.m_func))
  {
    other.commit();
  }

  ~ScopeGuard()
  {
    if (!m_commit)
      m_func();
  }

  void commit() const noexcept
  {
    m_commit = true;
  }

private:
  std::function<void()> m_func;
  mutable bool m_commit = false;
};

struct CommitFlag
{
  mutable bool m_commit = false;
  CommitFlag() = default;
  CommitFlag(CommitFlag &&other) noexcept : m_commit(std::exchange(other.m_commit, true)) {}
  void commit() const noexcept { m_commit = true; }
};

template <std::size_t S = 16>
class ScopeGuardFast : public CommitFlag
{
  alignas(8) char m_space[S];
  using guard_t = void (*)(void *);
  guard_t m_guard = nullptr;

  template <typename Callable>
  static void invoke(void *callable)
  {
    (*static_cast<Callable *>(callable))();
  }

public:
  template <typename Callable, typename D = std::decay_t<Callable>>
  ScopeGuardFast(Callable &&callable)
      : m_guard(invoke<Callable>)
  {
    static_assert(sizeof(Callable) <= sizeof(m_space));
    ::new (static_cast<void *>(m_space)) D(std::forward<Callable>(callable));
  }

  ScopeGuardFast(ScopeGuardFast &&other) = default;

  ~ScopeGuardFast()
  {
    if (!m_commit)
      m_guard(m_space);
  }
};

int main()
{
  try
  {
    constexpr std::size_t N = 100000;

    {
      Timer timer("type erased", []()
                  { int *p = nullptr;
        ScopeGuard SG{
            [&]
            { delete p; }};
        p = std::rand() < 0 ? new int(42) : nullptr; }, N);
    }

    {
      Timer timer("type erased fast", []()
                  { int *p = nullptr;
        ScopeGuardFast SG{
            [&]
            { delete p; }};
        p = std::rand() < 0 ? new int(42) : nullptr; }, N);
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}
