#include <iostream>
#include <string>
#include <string_view>
#include <cassert>

class VisitorBase
{
public:
  virtual ~VisitorBase() = default;
};

template <typename Visitable>
class Visitor
{
public:
  virtual void visit(Visitable *visitable) = 0;
};

class Pet
{
public:
  explicit Pet(std::string_view name) : m_name(name) {}
  const std::string &getName() const { return m_name; }

  virtual ~Pet() = default;

  virtual void accept(VisitorBase &visitor) = 0;

private:
  std::string m_name;
};

template <typename Visitable>
class VisitablePet : public Pet
{
public:
  using Pet::Pet;

  void accept(VisitorBase &visitor) override
  {
    if (auto pv = dynamic_cast<Visitor<Visitable> *>(&visitor))
      pv->visit(static_cast<Visitable *>(this));
    else
      assert(false);
  }
};

class Cat : public VisitablePet<Cat>
{
public:
  using VisitablePet<Cat>::VisitablePet;
};

class Dog : public VisitablePet<Dog>
{
public:
  using VisitablePet<Dog>::VisitablePet;
};

class FeedingVisitor : public VisitorBase, public Visitor<Cat>, public Visitor<Dog>
{
public:
  void visit(Cat *cat) override { std::cout << "Feeding cat: " << cat->getName() << std::endl; }
  void visit(Dog *dog) override { std::cout << "Feeding dog: " << dog->getName() << std::endl; }
};

class BathingVisitor : public VisitorBase, public Visitor<Cat>
{
public:
  void visit(Cat *cat) override { std::cout << "Bathing cat: " << cat->getName() << std::endl; }
};

int main()
{
  std::unique_ptr<Pet> pCat = std::make_unique<Cat>("Mittens");
  std::unique_ptr<Pet> pDog = std::make_unique<Dog>("Rex");

  FeedingVisitor feedingVisitor;
  BathingVisitor bathingVisitor;

  pCat->accept(feedingVisitor); // Feeding cat: Mittens
  pDog->accept(feedingVisitor); // Feeding dog: Rex

  pCat->accept(bathingVisitor); // Bathing cat: Mittens

  // Error: BathingVisitor doesn't support bathing Dog
  // pDog->accept(bathingVisitor);
}
