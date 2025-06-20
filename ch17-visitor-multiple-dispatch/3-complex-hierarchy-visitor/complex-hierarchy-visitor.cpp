#include <string_view>
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

class Pet;
class Cat;
class Dog;

class PetVisitor
{
public:
  virtual void visit(Dog *dog) = 0;
  virtual void visit(Cat *cat) = 0;

  virtual ~PetVisitor() = default;
};

class Pet
{
public:
  Pet(std::string_view name) : m_name(name) {}
  const std::string &getName() const { return m_name; }

  virtual void accept(class PetVisitor &visitor) = 0;

  virtual ~Pet() = default;

private:
  std::string m_name;
};

class Dog : public Pet
{
public:
  using Pet::Pet;

  void accept(class PetVisitor &visitor)
  {
    visitor.visit(this);
  }
};

class Cat : public Pet
{
public:
  using Pet::Pet;

  void accept(class PetVisitor &visitor)
  {
    visitor.visit(this);
  }
};

class FeedVisitor : public PetVisitor
{
public:
  void visit(Dog *dog) override
  {
    std::cout << "Feeding dog: " << dog->getName() << std::endl;
  }

  void visit(Cat *cat) override
  {
    std::cout << "Feeding cat: " << cat->getName() << std::endl;
  }
};

class PlayVisitor : public PetVisitor
{
public:
  void visit(Dog *dog) override
  {
    std::cout << "Playing with dog: " << dog->getName() << std::endl;
  }

  void visit(Cat *cat) override
  {
    std::cout << "Playing with cat: " << cat->getName() << std::endl;
  }
};

class Shelter
{
public:
  void addPet(Pet *pet)
  {
    m_pets.emplace_back(pet);
  }

  void accept(PetVisitor &visitor)
  {
    for (const auto &pet : m_pets)
      pet->accept(visitor);
  }

private:
  std::vector<std::unique_ptr<Pet>> m_pets;
};

int main()
{
  Shelter shelter;
  shelter.addPet(new Cat("Buddy"));
  shelter.addPet(new Cat("Whiskers"));

  FeedVisitor feedVisitor;
  shelter.accept(feedVisitor);

  PlayVisitor playVisitor;
  shelter.accept(playVisitor);

  return 0;
}
