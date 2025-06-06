#include <iostream>

template <typename T, typename F>
concept Deleter = requires(F f, T *p) { { f(p) } -> std::same_as<void>; };

struct DeleteByOperator
{
  template <typename T>
  void operator()(T *p) { delete p; }
};

struct MoveForbidden
{
  MoveForbidden() = default;
  MoveForbidden(const MoveForbidden &) = default;
  MoveForbidden &operator=(const MoveForbidden &) = default;

  MoveForbidden(MoveForbidden &&) = delete;            // no move
  MoveForbidden &operator=(MoveForbidden &&) = delete; // no move assignment
};

struct MoveAllowed
{
};

template <typename T, typename DeletionPolicy = DeleteByOperator, typename MovePolicy = MoveForbidden>
  requires Deleter<T, DeletionPolicy>
class SmartPtr : private DeletionPolicy, private MovePolicy
{
public:
  using value_type = T;
  using deletion_policy_type = DeletionPolicy;
  using move_policy_type = MovePolicy;

  explicit SmartPtr(T *p = nullptr, DeletionPolicy &&deletionPolicy = DeletionPolicy{})
      : DeletionPolicy(std::move(deletionPolicy)), m_p(p)
  {
  }

  ~SmartPtr()
  {
    DeletionPolicy::operator()(m_p);
  }

  SmartPtr(const SmartPtr &) = delete;            // no copy
  SmartPtr &operator=(const SmartPtr &) = delete; // no copy assignment

  SmartPtr(SmartPtr &&other) noexcept
      : DeletionPolicy(std::move(other)) // transfer deletion policy
        ,
        MovePolicy(std::move(other)) // transfer move policy
  {
    m_p = std::exchange(other.m_p, nullptr);
  }

  SmartPtr &operator=(SmartPtr &&other) noexcept
  {
    if (this != &other)
    {
      DeletionPolicy::operator()(m_p);             // transfer deletion policy
      delete m_p;                                  // delete current resource
      m_p = std::exchange(other.m_p, nullptr);     // take ownership of other's resource
      DeletionPolicy::operator=(std::move(other)); // transfer deletion policy
      MovePolicy::operator=(std::move(other));     // transfer move policy
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
  SmartPtr<int, DeleteByOperator> p1(new int(42));

  using ptr_t = decltype(p1);

  SmartPtr<ptr_t::value_type, ptr_t::deletion_policy_type, ptr_t::move_policy_type> p2;
  SmartPtr<double,            ptr_t::deletion_policy_type, ptr_t::move_policy_type> p3;

  return 0;
}
