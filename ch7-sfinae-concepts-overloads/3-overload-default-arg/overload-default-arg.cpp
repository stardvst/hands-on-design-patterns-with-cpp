#include <iostream>

void f(int i) { std::cout << "f(int)\n"; }
void f(long i, long j) { std::cout << "f(long2)\n"; }
void f(double d, double = 0) { std::cout << "f(double, double = 0)\n"; }

int main()
{
  f(5);
  f(5L, 5);
  // f(5, 5); // ambiguous
  f(5.0);
  f(5.0f);
  // f(5L); // ambiguous

  return 0;
}
