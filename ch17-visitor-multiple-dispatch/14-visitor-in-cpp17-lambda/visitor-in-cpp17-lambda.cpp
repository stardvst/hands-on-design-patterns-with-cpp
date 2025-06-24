#include <variant>
#include <iostream>
#include <string>

class PetBase
{
public:
  explicit PetBase(const std::string &name) : m_name(name) {}
  const std::string &name() const { return m_name; }

  virtual ~PetBase() = default;

private:
  std::string m_name;
};

class Cat : private PetBase
{
public:
  using PetBase::name;
  using PetBase::PetBase;
};

class Dog : private PetBase
{
public:
  using PetBase::name;
  using PetBase::PetBase;
};

// Not from Pet hierarchy
class Lorikeet
{
public:
  Lorikeet(std::string_view body, std::string_view head)
      : m_body(body), m_head(head) {}

  std::string color() const
  {
    return m_body + " " + m_head;
  }

private:
  std::string m_body;
  std::string m_head;
};

#define IS_SAME(v, T) \
  std::is_same_v<std::decay_t<decltype(v)>, T>

int main()
{
  auto fv = [](const auto &animal)
  {
    if constexpr (IS_SAME(animal, Cat))
    {
      std::cout << "Feeding cat: " << animal.name() << std::endl;
    }
    else if constexpr (IS_SAME(animal, Dog))
    {
      std::cout << "Feeding dog: " << animal.name() << std::endl;
    }
    else if constexpr (IS_SAME(animal, Lorikeet))
    {
      std::cout << "Feeding lorikeet: " << animal.color() << std::endl;
    }
  };

  Cat cat("Whiskers");
  Dog dog("Rex");
  Lorikeet lorikeet("green", "blue");

  std::variant<Cat, Dog, Lorikeet> pet = cat;

  std::visit(fv, pet);

  pet = dog;
  std::visit(fv, pet);

  pet = lorikeet;
  std::visit(fv, pet);

  return 0;
}
