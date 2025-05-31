#include <iostream>

template <typename T>
class SmartPtr
{
public:
  explicit SmartPtr(T *p = nullptr) : m_p(p) {}
  ~SmartPtr() { delete m_p; }

  SmartPtr(const SmartPtr &) = delete;            // no copy
  SmartPtr &operator=(const SmartPtr &) = delete; // no copy assignment

  SmartPtr(SmartPtr &&other) noexcept
  {
    m_p = std::exchange(other.m_p, nullptr);
  }

  SmartPtr &operator=(SmartPtr &&other) noexcept
  {
    if (this != &other)
    {
      delete m_p;                              // delete current resource
      m_p = std::exchange(other.m_p, nullptr); // take ownership of other's resource
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
  SmartPtr<int> p1(new int(42));
  SmartPtr<int> p2 = std::move(p1); // Move ownership

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
