#include <iostream>

class Pet
{
public:
  virtual ~Pet() = default;

  explicit Pet(std::string_view name) : m_name(name) {}
  const std::string &getName() const { return m_name; }

  template <typename Visitable, typename Visitor>
  static void accept(Visitable &visitable, Visitor &visitor)
  {
    visitor.visit(visitable);
  }

private:
  std::string m_name;
};

class Cat : public Pet
{
public:
  using Pet::Pet;
};

class Dog : public Pet
{
public:
  using Pet::Pet;
};

class FeedingVisitor
{
public:
  void visit(Cat &cat) { std::cout << "Feeding cat: " << cat.getName() << std::endl; }
  void visit(Dog &dog) { std::cout << "Feeding dog: " << dog.getName() << std::endl; }
};

class BathingVisitor
{
public:
  void visit(Cat &cat) { std::cout << "Bathing cat: " << cat.getName() << std::endl; }
};

int main()
{
  Cat cat("Cat");
  Dog dog("Dog");

  FeedingVisitor feedingVisitor;
  BathingVisitor bathingVisitor;

  Pet::accept(cat, feedingVisitor); // Feeding cat: Cat
  Pet::accept(dog, feedingVisitor); // Feeding dog: Dog

  Pet::accept(cat, bathingVisitor); // Bathing cat: Cat

  // Error: BathingVisitor doesn't support bathing Dog
  // Pet::accept(dog, bathingVisitor);
}
