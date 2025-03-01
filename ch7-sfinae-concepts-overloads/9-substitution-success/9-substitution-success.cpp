#include <iostream>

template <typename T>
T *f(T i, T &j)
{
  j = 2 * i;
  return new T(i);
}

int main()
{
  int i = 5, j = 7;
  const int *p = f(i, j);
  std::cout << "i = " << i << ", j = " << j << ", *p = " << *p << '\n';

  return 0;
}
