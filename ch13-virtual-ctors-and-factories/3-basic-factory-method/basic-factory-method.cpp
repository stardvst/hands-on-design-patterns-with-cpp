#include <iostream>

enum Buildings {
    HOUSE,
    OFFICE,
    SHOP
};

class Building {
public:
    virtual ~Building() {}
    virtual void build() = 0;
};

class House : public Building {
public:
    void build() override {
        std::cout << "Building a house." << std::endl;
    }
};

class Office : public Building {
public:
    void build() override {
        std::cout << "Building an office." << std::endl;
    }
};

class Shop : public Building {
public:
    void build() override {
        std::cout << "Building a shop." << std::endl;
    }
};

std::unique_ptr<Building> makeBuilding(Buildings type)
{
  switch (type)
  {
  case HOUSE:
    return std::make_unique<House>();
  case OFFICE:
    return std::make_unique<Office>();
  case SHOP:
    return std::make_unique<Shop>();
  default:
    return nullptr;
  }
}

int main()
{
    auto house = makeBuilding(HOUSE);
    house->build();

    auto office = makeBuilding(OFFICE);
    office->build();

    auto shop = makeBuilding(SHOP);
    shop->build();

    return 0;
}
