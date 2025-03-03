#include <iostream>

template <typename U, typename V>
auto compute(U pu, V pv) -> decltype(bool(*pu == *pv)) // no matching function for call to 'compute'
{
  return *pu < *pv; // invalid operands to binary expression ('A' and 'int')
}

struct A {};

int main()
{
  A a;
  int i{};
  double d{};

  compute(&d, &i);
  // compute(&a, &i); // no matching function for call to 'compute'

  return 0;
}
