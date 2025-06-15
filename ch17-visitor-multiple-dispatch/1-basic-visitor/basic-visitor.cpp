#include <string_view>
#include <string>
#include <iostream>

class Cat;
class Dog;

class PetVisitor
{
public:
  virtual void visit(const Dog &dog) = 0;
  virtual void visit(const Cat &cat) = 0;

  virtual ~PetVisitor() = default;
};

class Pet
{
public:
  Pet(std::string_view name) : m_name(name) {}
  const std::string &getName() const { return m_name; }

  virtual void accept(class PetVisitor &visitor) const = 0;

  virtual ~Pet() = default;

private:
  std::string m_name;
};

class Dog : public Pet
{
public:
  using Pet::Pet;

  void accept(class PetVisitor &visitor) const
  {
    visitor.visit(*this);
  }
};

class Cat : public Pet
{
public:
  using Pet::Pet;

  void accept(class PetVisitor &visitor) const
  {
    visitor.visit(*this);
  }
};

class FeedVisitor : public PetVisitor
{
public:
  void visit(const Dog &dog) override
  {
    std::cout << "Feeding dog: " << dog.getName() << std::endl;
  }

  void visit(const Cat &cat) override
  {
    std::cout << "Feeding cat: " << cat.getName() << std::endl;
  }
};

class PlayVisitor : public PetVisitor
{
public:
  void visit(const Dog &dog) override
  {
    std::cout << "Playing with dog: " << dog.getName() << std::endl;
  }

  void visit(const Cat &cat) override
  {
    std::cout << "Playing with cat: " << cat.getName() << std::endl;
  }
};

int main()
{
  Dog dog("Buddy");
  Cat cat("Whiskers");

  FeedVisitor feedVisitor;
  PlayVisitor playVisitor;

  dog.accept(feedVisitor); // Feeding dog: Buddy
  cat.accept(feedVisitor); // Feeding cat: Whiskers
  dog.accept(playVisitor); // Playing with dog: Buddy
  cat.accept(playVisitor); // Playing with cat: Whiskers

  ////////////////////////////
  // polymorphic visitor example
  std::unique_ptr<Pet> pCat = std::make_unique<Cat>("Mittens");
  std::unique_ptr<Pet> pDog = std::make_unique<Dog>("Rex");

  pCat->accept(feedVisitor); // Feeding cat: Mittens
  pDog->accept(feedVisitor); // Feeding dog: Rex

  /////////////////////////////
  // double dispatch example
  std::unique_ptr<Pet> pPet = std::make_unique<Dog>("Max");
  std::unique_ptr<PetVisitor> pVisitor = std::make_unique<PlayVisitor>();
  pPet->accept(*pVisitor); // Playing with dog: Max

  return 0;
}
