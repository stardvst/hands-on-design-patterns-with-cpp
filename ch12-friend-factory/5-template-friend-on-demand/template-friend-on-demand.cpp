#include <iostream>

namespace NS
{

  template <typename T>
  class C
  {
  public:
    C(T x) : m_x(x) {}

    friend C operator+(const C &lhs, const C &rhs)
    {
      return C(lhs.m_x + rhs.m_x);
    }

    friend std::ostream &operator<<(std::ostream &os, const C &c)
    {
      os << c.m_x;
      return os;
    }

  private:
    T m_x;
  };

}

int main()
{
  NS::C<int> a(1);
  NS::C<int> b(2);
  NS::C<int> c = a + b; // Calls the template operator+
  std::cout << c << std::endl;

  NS::C<int> d = a + 3;
  std::cout << d << std::endl;

  NS::C<int> e = 3 + a;
  std::cout << e << std::endl;

  NS::C<int> f = 1 + 2;
  std::cout << f << std::endl;

  // both add and print the objects
  std::cout << (a + b) << std::endl;
}
