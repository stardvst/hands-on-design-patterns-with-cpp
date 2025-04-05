#include <iostream>

template <typename T>
class Collection
{
public:
  T filter() // "this" is T, not a Collection
  {
    std::cout << "Collection::filter()\n";
    return T{};
  }

  T sort() // "this" is T, not a Collection
  {
    std::cout << "Collection::sort()\n";
    return T{};
  }
};

class SortedCollection : public Collection<SortedCollection>
{
public:
  SortedCollection search()
  {
    std::cout << "SortedCollection::search()\n";
    return SortedCollection{};
  }

  SortedCollection median()
  {
    std::cout << "SortedCollection::median()\n";
    return SortedCollection{};
  }
};

int main()
{
  Collection<SortedCollection> sc;
  sc.filter().sort().search().median();
}
