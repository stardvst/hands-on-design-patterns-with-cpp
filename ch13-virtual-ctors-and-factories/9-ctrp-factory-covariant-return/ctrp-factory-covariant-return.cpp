#include <memory>

template <typename Derived>
class Base
{
public:
  virtual ~Base() = default;
  Base()
  {
    static_assert(std::is_base_of<Base, Derived>::value,
                  "Derived must inherit from Base");
  }

  std::unique_ptr<Derived> clone() const
  {
    return std::make_unique<Derived>(static_cast<const Derived &>(*this));
  }
};

class Derived : public Base<Derived>
{
};

int main()
{
  std::unique_ptr<Derived> d = std::make_unique<Derived>();
  std::unique_ptr<Derived> dClone = d->clone();
  return 0;
}
