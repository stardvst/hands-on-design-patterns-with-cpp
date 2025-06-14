#include <iostream>
#include <memory>
#include <cassert>

class SmallHeap
{
public:
  void *allocate(size_t s)
  {
    assert(s <= m_size);
    return m_memory;
  }

  void deallocate(void *p)
  {
    assert(p == m_memory);
  }

  SmallHeap() = default;
  SmallHeap(const SmallHeap &) = delete;
  SmallHeap &operator=(const SmallHeap &) = delete;

private:
  static constexpr size_t m_size = 1024;
  char m_memory[m_size];
};

template <typename T>
struct DeleteByOperator
{
  void operator()(T *p) { delete p; }
};

template <typename T, typename Heap>
struct DeleteByHeap
{
  explicit DeleteByHeap(Heap &heap) : m_heap(heap) {}

  void operator()(T *p)
  {
    p->~T();
    m_heap.deallocate(p);
  }

private:
  Heap &m_heap;
};

template <typename T, template <typename> class DeletionPolicy = DeleteByOperator>
class SmartPtr
{
public:
  explicit SmartPtr(T *p = nullptr, DeletionPolicy<T> &&deletionPolicy = DeletionPolicy<T>{})
      : m_p(p), m_deletionPolicy(std::move(deletionPolicy)) {}

  ~SmartPtr()
  {
    m_deletionPolicy(m_p);
  }

  SmartPtr(const SmartPtr &) = delete;            // no copy
  SmartPtr &operator=(const SmartPtr &) = delete; // no copy assignment

  SmartPtr(SmartPtr &&other) noexcept
      : m_p(std::exchange(other.m_p, nullptr)), m_deletionPolicy(std::move(other.m_deletionPolicy)) {}

  SmartPtr &operator=(SmartPtr &&other) noexcept
  {
    if (this != &other)
    {
      m_deletionPolicy(m_p);                                // delete current resource
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
  DeletionPolicy<T> m_deletionPolicy;
};

template <typename T>
struct DeleteMyHeap : public DeleteByHeap<T, SmallHeap>
{
  using DeleteByHeap<T, SmallHeap>::DeleteByHeap; // Inherit constructor
};

// OR

template <typename T>
using DeleteMyHeapT = DeleteByHeap<T, SmallHeap>;

int main()
{
  SmallHeap heap;
  SmartPtr<int, DeleteMyHeap> p(new (&heap) int, DeleteMyHeap<int>(heap));

  if (p)
  {
    std::cout << "Value: " << *p << std::endl; // Should print 42
  }
  else
  {
    std::cout << "Pointer is empty." << std::endl;
  }

  return 0;
}
