#include <iostream>

void f(int i, int j) { std::cout << "f(int2)\n"; }
void f(long i, long j) { std::cout << "f(long2)\n"; }
void f(double d) { std::cout << "f(double)\n"; }

int main()
{
  f(5, 5);
  f(5L, 5L);
  f(5, 5.0);
  // f(5, 5L); // ambiguous

  return 0;
}
