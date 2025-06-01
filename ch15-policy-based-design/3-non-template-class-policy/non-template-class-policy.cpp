#include <iostream>

template <typename T, typename F>
concept Deleter = requires(F f, T *p) { { f(p) } -> std::same_as<void>; };

// non-template class, with template operator()
struct DeleteByOperator
{
  template <typename T>
  void operator()(T *p) { delete p; }
};

template <typename T, typename DeletionPolicy = DeleteByOperator>
  requires Deleter<T, DeletionPolicy>
class SmartPtr
{
public:
  explicit SmartPtr(T *p = nullptr, DeletionPolicy &&deletionPolicy = DeletionPolicy{}) : m_p(p), m_deletionPolicy(std::move(deletionPolicy)) {}

  ~SmartPtr()
  {
    m_deletionPolicy(m_p);
  }

  SmartPtr(const SmartPtr &) = delete;            // no copy
  SmartPtr &operator=(const SmartPtr &) = delete; // no copy assignment

  SmartPtr(SmartPtr &&other) noexcept
  {
    m_p = std::exchange(other.m_p, nullptr);
    m_deletionPolicy = std::move(other.m_deletionPolicy); // transfer deletion policy
  }

  SmartPtr &operator=(SmartPtr &&other) noexcept
  {
    if (this != &other)
    {
      delete m_p;                                           // delete current resource
      m_p = std::exchange(other.m_p, nullptr);              // take ownership of other's resource
      m_deletionPolicy = std::move(other.m_deletionPolicy); // transfer deletion policy
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
  DeletionPolicy m_deletionPolicy;
};

int main()
{
  SmartPtr p1(new int(42));
  SmartPtr p2 = std::move(p1); // Move ownership

  if (p2)
  {
    std::cout << "Value: " << *p2 << std::endl; // Should print 42
  }
  else
  {
    std::cout << "Pointer is empty." << std::endl;
  }

  return 0;
}
