#include <iostream>

class City
{
public:
  class DayEvents
  {
    friend City;

    bool flood = false;
    bool fire = false;
    bool tornado = false;

    public:
      DayEvents() = default;
      DayEvents &setFlood(bool b) { flood = b; return *this; }
      DayEvents &setFire(bool b) { fire = b; return *this; }
      DayEvents &setTornado(bool b) { tornado = b; return *this; }
  };

  void day(DayEvents dayEvents)
  {
    std::cout << std::boolalpha;
    std::cout << "Flood: " << dayEvents.flood << '\n';
    std::cout << "Fire: " << dayEvents.fire << '\n';
    std::cout << "Tornado: " << dayEvents.tornado << '\n';
  }
};

int main()
{
  City city;
  city.day(City::DayEvents().setFlood(true).setFire(false));
}
