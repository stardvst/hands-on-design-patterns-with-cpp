#include <iostream>
#include <vector>

struct much_less
{
  template <typename T>
  bool operator()(const T &a, const T &b) const
  {
    return a < b && std::abs(a - b) > s_tolerance;
  }

  static constexpr double s_tolerance = 0.2;
};

template <typename RandomIt>
void sort_much_less(RandomIt first, RandomIt last)
{
  std::sort(first, last, much_less());
}

template <typename Container>
void sort_much_less(Container &c)
{
  std::sort(c.begin(), c.end(), much_less());
}

int main()
{
  std::vector<double> v = {10, 1.1, 1.3, 4.5, 1.2, 4.8};
  sort_much_less(v.begin(), v.end());

  for (auto x : v)
    std::cout << x << ' ';
  std::cout << std::endl;

  v = {10, 1.1, 1.3, 4.5, 1.2, 4.8};
  sort_much_less(v);

  for (auto x : v)
    std::cout << x << ' ';
  std::cout << std::endl;
}
