#include <iostream>

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

int g(int i, int j)
{
  return i + j;
}

void nonReturningFunction(int i, int j)
{
  std::cout << "Non-returning function called with " << i << " and " << j << "." << std::endl;
}

struct S
{
  int operator()(int i, int j) const
  {
    return i * j;
  }
};

int main()
{
  auto add = [](int a, int b)
  { return a + b; };
  auto debugAdd = decorateDebug(add, "add");
  debugAdd(3, 4);

  auto gDebug = decorateDebug(g, "g");
  gDebug(5, 6);

  auto sDebug = decorateDebug(S{}, "S::operator()");
  sDebug(7, 8);

  auto nonReturningDebug = decorateDebug(nonReturningFunction, "nonReturningFunction");
  nonReturningDebug(9, 10);

  return 0;
}
