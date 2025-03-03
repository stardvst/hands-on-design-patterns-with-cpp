#include <iostream>
#include <vector>

template <typename T, typename V>
bool find(const T &cont, V value)
{
  for (typename T::value_type i : cont)
    if (i == value)
      return true;
  return false;
}

template <typename U, typename V>
auto compute(U u, V v)
{
  std::common_type_t<U, V> res = (u > v) ? u : v;
  return res;
}

struct A {};

int main()
{
  A a;
  int i{};
  double d{};
  std::vector<int> v;

  find(v, 5);
  // find(i, 5); // type 'int' cannot be used prior to '::' because it has no members

  compute(d, i);
  // compute(a, i); // no type named 'type' in 'std::common_type<A, int>'

  return 0;
}
