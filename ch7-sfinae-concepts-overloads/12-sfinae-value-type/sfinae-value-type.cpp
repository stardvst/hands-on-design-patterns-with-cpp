#include <iostream>

template <std::size_t N>
void f(char (*)[N % 2] = nullptr)
{
  std::cout << "N = " << N << " is odd\n";
}

template <std::size_t N>
void f(char (*)[1 - N % 2] = nullptr)
{
  std::cout << "N = " << N << " is even\n";
}

int main()
{
  f<5>();
  f<6>();

  return 0;
}
