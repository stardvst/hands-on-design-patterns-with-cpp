#include <iostream>

template <typename D>
class B
{
public:
  friend bool operator!=(const B &lhs, const B &rhs)
  {
    return !(lhs == rhs);
  }
};

template <typename T>
class C : public B<C<T>>
{
public:
  C(T x) : m_x(x) {}

  friend bool operator==(const C &lhs, const C &rhs)
  {
    return lhs.m_x == rhs.m_x;
  }

private:
  T m_x;
};

int main()
{
  C<int> a(1);
  C<int> b(2);
  std::cout << (a == b) << std::endl;
  std::cout << (a != b) << std::endl;
}
