#include <iostream>

template <typename D>
class B
{
public:
  friend D operator+(const D &lhs, const D &rhs)
  {
    D res(lhs);
    res += rhs; // convert += to +
    return res;
  }

  friend std::ostream &operator<<(std::ostream &os, const D &d)
  {
    d.print(os);
    return os;
  }
};

template <typename T>
class C : public B<C<T>>
{
public:
  C(T x) : m_x(x) {}

  C operator+=(const C &incr)
  {
    m_x += incr.m_x;
    return *this;
  }

  void print(std::ostream &os) const
  {
    os << m_x;
  }

private:
  T m_x;
};

int main()
{
  C<int> a(1);
  C<int> b(2);
  std::cout << (a + b) << std::endl;
}
