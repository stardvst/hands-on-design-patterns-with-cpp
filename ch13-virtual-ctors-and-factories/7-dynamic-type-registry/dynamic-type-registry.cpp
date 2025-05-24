#include <iostream>
#include <cstdlib>
#include <memory>
#include <functional>
#include <vector>

class Building
{
private:
  static std::size_t s_buildingTypeCount;

  using BuildingFactory = std::function<std::unique_ptr<Building>()>;
  static std::vector<BuildingFactory> s_buildingFactories;

public:
  virtual ~Building() = default;

  static std::size_t RegisterBuilding(BuildingFactory factory)
  {
    s_buildingFactories.push_back(std::move(factory));
    return s_buildingTypeCount++;
  }

  static std::unique_ptr<Building> CreateBuilding(std::size_t type)
  {
    BuildingFactory factory = s_buildingFactories[type];
    return std::unique_ptr<Building>(factory());
  }
};

std::size_t Building::s_buildingTypeCount = 0;
std::vector<Building::BuildingFactory> Building::s_buildingFactories;

/////////////////////
// Example buildings
//

class Forge : public Building
{
public:
  Forge() { std::cout << "new Forge" << std::endl; }

  static std::unique_ptr<Building> MakeForge()
  {
    return std::make_unique<Forge>();
  }

  static const std::size_t typeTag;
};

const std::size_t Forge::typeTag = Building::RegisterBuilding(Forge::MakeForge);

class Farm : public Building
{
public:
  Farm() { std::cout << "new Farm" << std::endl; }
  static const std::size_t typeTag;
};

const std::size_t Farm::typeTag = Building::RegisterBuilding([]()
                                                             { return std::make_unique<Farm>(); });

int main()
{
  std::unique_ptr<Building> forge = Building::CreateBuilding(Forge::typeTag);
  std::unique_ptr<Building> farm = Building::CreateBuilding(Farm::typeTag);
}
