#include <iostream>
#include <vector>

template <typename T>
bool find(const T &cont, typename T::value_type value) { return true; }

template <typename U, typename V>
std::common_type_t<U, V> compute(U, V) { return {}; }

template <typename T>
auto process(T p) -> decltype(*p) { return *p; }

struct A {};

int main()
{
  A a;
  int i{};
  double d{};
  std::vector<int> v;
  int *p = &i;

  find(v, 5);
  // find(i, 5); // no matching function for call to 'find'

  compute(d, i);
  // compute(a, i); // no matching function for call to 'compute'

  process(p);
  // process(i); // no matching function for call to 'process'

  return 0;
}
