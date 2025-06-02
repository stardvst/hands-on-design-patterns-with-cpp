#include <iostream>

template <typename T, typename F>
concept Deleter = requires(F f, T *p) { { f(p) } -> std::same_as<void>; };

template <typename T>
struct DeleteByOperator
{
  void operator()(T *p) { delete p; }
};

template <typename T>
struct WithRelease
{
  void release()
  {
    static_cast<T *>(this)->m_p = nullptr;
  }
};

template <typename T>
struct NoRelease
{
};

template <typename T, typename DeletionPolicy = DeleteByOperator<T>, template <typename...> class ReleasePolicy = WithRelease>
  requires Deleter<T, DeletionPolicy>
class SmartPtr : private DeletionPolicy, public ReleasePolicy<SmartPtr<T, DeletionPolicy, ReleasePolicy>>
{
  friend class ReleasePolicy<SmartPtr>;

public:
  explicit SmartPtr(T *p = nullptr, DeletionPolicy &&deletionPolicy = DeletionPolicy{})
      : DeletionPolicy(std::move(deletionPolicy)), m_p(p) {}

  ~SmartPtr()
  {
    DeletionPolicy::operator()(m_p);
  }

  SmartPtr(const SmartPtr &) = delete;            // no copy
  SmartPtr &operator=(const SmartPtr &) = delete; // no copy assignment

  SmartPtr(SmartPtr &&other) noexcept
      : DeletionPolicy(std::move(other)) // transfer deletion policy
        ,
        ReleasePolicy<SmartPtr>(std::move(other)) // transfer release policy
  {
    m_p = std::exchange(other.m_p, nullptr);
  }

  SmartPtr &operator=(SmartPtr &&other) noexcept
  {
    if (this != &other)
    {
      delete m_p;                              // delete current resource
      m_p = std::exchange(other.m_p, nullptr); // take ownership of other's resource
      static_cast<DeletionPolicy &>(*this) = std::move(other);
      static_cast<ReleasePolicy<SmartPtr> &>(*this) = std::move(other);
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
  SmartPtr p1(new int(42));
  p1.release();

  SmartPtr<int, DeleteByOperator<int>, NoRelease> p2(new int(84), DeleteByOperator<int>{});
  // p2.release(); // error: no member named 'release' in 'SmartPtr<int, DeleteByOperator<int>, NoRelease>'

  return 0;
}
