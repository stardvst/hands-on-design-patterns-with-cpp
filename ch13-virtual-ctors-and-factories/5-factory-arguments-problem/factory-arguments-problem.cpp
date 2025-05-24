#include <iostream>

enum Buildings
{
  FARM,
  FORGE
};

class Building
{
public:
  virtual ~Building() {}
  virtual void build() = 0;
};

class Farm : public Building
{
public:
  explicit Farm(double size) {}

  void build() override
  {
    std::cout << "Building a farm." << std::endl;
  }
};

class Forge : public Building
{
public:
  static constexpr std::size_t weaponsmith = 0x1;
  static constexpr std::size_t welder = 0x2;
  static constexpr std::size_t farrier = 0x4;

  explicit Forge(std::size_t staff, std::size_t services) {}

  void build() override
  {
    std::cout << "Building an forge." << std::endl;
  }
};

template <typename ...Args>
std::unique_ptr<Building> makeBuilding(Buildings type, Args&&... args)
{
  switch (type)
  {
  case FARM:
    return std::make_unique<Farm>(std::forward<Args>(args)...);
  case FORGE:
    return std::make_unique<Forge>(std::forward<Args>(args)...);
  default:
    return nullptr;
  }
}

int main()
{
  // error: no matching constructor for initialization of 'Farm'
  // because the code creaating a Farm must still compile
  auto forge = makeBuilding(FORGE, 2, Forge::weaponsmith | Forge::welder | Forge::farrier);
  forge->build();
}
