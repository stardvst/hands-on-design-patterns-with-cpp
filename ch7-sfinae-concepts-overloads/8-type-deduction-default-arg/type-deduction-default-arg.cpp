#include <iostream>

void f(int i, int j = 1L)
{
  std::cout << "f(int, int)\n";
}

template <typename T>
void f(T i, T *p = nullptr)
{
  std::cout << "f(T, T*)\n";
}

int main()
{
  int i{};
  f(5);
  f(5L);

  return 0;
}
