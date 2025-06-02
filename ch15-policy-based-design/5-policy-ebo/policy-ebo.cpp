#include <iostream>

template <typename T, typename F>
concept Deleter = requires(F f, T *p) { { f(p) } -> std::same_as<void>; };

template <typename T>
struct DeleteByOperator
{
  void operator()(T *p) { delete p; }
};

template <typename T, template <typename...> class DeletionPolicy = DeleteByOperator>
  requires Deleter<T, DeletionPolicy<T>>
class SmartPtr : private DeletionPolicy<T>
{
public:
  explicit SmartPtr(T *p = nullptr, DeletionPolicy<T> &&deletionPolicy = DeletionPolicy<T>{})
      : DeletionPolicy<T>(std::move(deletionPolicy)), m_p(p) {}

  ~SmartPtr()
  {
    DeletionPolicy<T>::operator()(m_p);
  }

  SmartPtr(const SmartPtr &) = delete;            // no copy
  SmartPtr &operator=(const SmartPtr &) = delete; // no copy assignment

  SmartPtr(SmartPtr &&other) noexcept
      : DeletionPolicy<T>(std::move(other)) // transfer deletion policy
  {
    m_p = std::exchange(other.m_p, nullptr);
  }

  SmartPtr &operator=(SmartPtr &&other) noexcept
  {
    if (this != &other)
    {
      delete m_p;                                           // delete current resource
      m_p = std::exchange(other.m_p, nullptr);              // take ownership of other's resource
      DeletionPolicy<T>::operator()(m_p);                   // transfer deletion policy
    }
    return *this;
  }

  T *operator->() { return m_p; }
  const T *operator->() const { return m_p; }

  T &operator&() { return *m_p; }
  const T &operator*() const { return *m_p; }

  operator bool() const { return m_p != nullptr; }

private:
  T *m_p{nullptr};
};

int main()
{
  SmartPtr p(new int(42));

  std::cout << "sizeof SmartPtr: " << sizeof(p) << std::endl; // Should print 42

  return 0;
}
