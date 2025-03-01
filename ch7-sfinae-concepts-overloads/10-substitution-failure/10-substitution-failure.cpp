#include <iostream>

template <typename T>
void f(T i, typename T::t &j)
{
  std::cout << "f(T, typename T::t)\n";
}

template <typename T>
void f(T i, T j)
{
  std::cout << "f(T, T)\n";
}

struct A
{
  struct t
  {
    int i;
  };

  t i;
};

int main()
{
  A a{5};
  f(a, a.i);

  f(5, 7);

  return 0;
}
