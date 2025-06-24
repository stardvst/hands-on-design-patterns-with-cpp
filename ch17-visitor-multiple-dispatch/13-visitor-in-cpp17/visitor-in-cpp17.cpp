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

class FeedingVisitor
{
public:
  void operator()(const Cat &cat)
  {
    std::cout << "Feeding cat: " << cat.name() << std::endl;
  }
  void operator()(const Dog &dog)
  {
    std::cout << "Feeding dog: " << dog.name() << std::endl;
  }
  void operator()(const Lorikeet &lorikeet)
  {
    std::cout << "Feeding lorikeet: " << lorikeet.color() << std::endl;
  }
};

int main()
{
  Cat cat("Whiskers");
  Dog dog("Rex");
  Lorikeet lorikeet("green", "blue");

  std::variant<Cat, Dog, Lorikeet> pet = cat;

  std::visit(FeedingVisitor(), pet);

  pet = dog;
  std::visit(FeedingVisitor(), pet);

  pet = lorikeet;
  std::visit(FeedingVisitor(), pet);

  return 0;
}
