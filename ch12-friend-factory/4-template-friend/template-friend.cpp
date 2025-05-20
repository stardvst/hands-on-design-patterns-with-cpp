#include <iostream>

template <typename T>
class C
{
public:
  C(T x) : m_x(x) {}

  void print() const
  {
    std::cout << m_x << std::endl;
  }

  template <typename U>
  friend C<U> operator+(const C<U> &lhs, const C<U> &rhs);

private:
  T m_x;
};

template <typename T>
C<T> operator+(const C<T> &lhs, const C<T> &rhs)
{
  return C<T>(lhs.m_x + rhs.m_x);
}

// don't do this!!!
// it accepts two arguments of ANY type
template <typename C>
C operator+(const C &lhs, const C &rhs)
{
  return C(lhs.m_x + rhs.m_x);
}

int main()
{
  C<int> a(1);
  C<int> b(2);
  C<int> c = a + b; // Calls the template operator+
  c.print();

  // C<int> d = a + 3; // Error: no matching function for call to 'operator+'
  // C<int> e = 3 + a; // Error: no matching function for call to 'operator+'
  // C<int> e = 1 + 2; // Error: no matching function for call to 'operator+'
}
