#include <iostream>

class C
{
public:
  C(int x) : m_x(x) {}

  friend int f(int i);
  friend int g(const C &c) { return c.m_x + C::s_n; }

private:
  static int s_n;
  int m_x{};
};

int C::s_n = 42;

int f(int i) { return i + C::s_n; }

int main()
{
  C c(1);
  std::cout << f(1) << std::endl; // now compiles
  std::cout << g(c) << std::endl;
}
