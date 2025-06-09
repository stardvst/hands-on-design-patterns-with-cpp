#include <iostream>
#include <mutex>

template <typename Callable>
class LockDecorator
{
public:
  template <typename F>
  LockDecorator(F &&f, std::mutex &mutex) : m_callable(std::forward<F>(f)), m_mutex(mutex) {}

  template <typename... Args>
  auto operator()(Args &&...args) const
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    using return_type = decltype(m_callable(std::forward<Args>(args)...));
    if constexpr (!std::is_same_v<return_type, void>)
    {
      return m_callable(std::forward<Args>(args)...);
    }
    else
    {
      m_callable(std::forward<Args>(args)...);
    }
  }

private:
  Callable m_callable;
  std::mutex &m_mutex;
};

template <typename Callable>
auto decorateLock(Callable &&callable, std::mutex &mutex)
{
  // Use std::decay_t to remove references and cv-qualifiers, to store it by value in the decorator.
  // This allows the decorator to work with both lvalue and rvalue callables (such as lambda functions).
  return LockDecorator<Callable>(std::forward<Callable>(callable), mutex);
}

template <typename Callable>
class DebugDecorator
{
public:
  template <typename F>
  DebugDecorator(F &&f, const char *title) : m_callable(std::forward<F>(f)), m_title(title) {}

  template <typename... Args>
  auto operator()(Args &&...args) const
  {
    std::cout << "Invoking " << m_title << "." << std::endl;

    using return_type = decltype(m_callable(std::forward<Args>(args)...));
    if constexpr (!std::is_same_v<return_type, void>)
    {
      auto result = m_callable(std::forward<Args>(args)...);
      std::cout << "Result: " << result << "." << std::endl;
      return result;
    }
    else
    {
      m_callable(std::forward<Args>(args)...);
    }
  }

private:
  Callable m_callable;
  const char *m_title{nullptr};
};

template <typename Callable>
auto decorateDebug(Callable &&callable, const char *title)
{
  // Use std::decay_t to remove references and cv-qualifiers, to store it by value in the decorator.
  // This allows the decorator to work with both lvalue and rvalue callables (such as lambda functions).
  return DebugDecorator<std::decay_t<Callable>>(std::forward<Callable>(callable), title);
}

std::mutex g_mutex;

int unsafeFunction(int i, int j)
{
  return i + j;
}

int main()
{
  auto safeFunction = decorateDebug(
      decorateLock(unsafeFunction, g_mutex), "safeFunction");
  safeFunction(1, 2); // This will lock the mutex before calling unsafeFunction

  return 0;
}
