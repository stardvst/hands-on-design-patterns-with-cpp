#include <iostream>

class Point
{
public:
  static Point createCartesian(double x, double y)
  {
    return Point(x, y);
  }

  static Point createPolar(double radius, double angle)
  {
    return Point(radius * cos(angle), radius * sin(angle));
  }

  void print() const
  {
    std::cout << "Point(" << m_x << ", " << m_y << ")" << std::endl;
  }

private:
  Point(double x, double y) : m_x(x), m_y(y) {}

  double m_x{};
  double m_y{};
};

class PointModern
{
public:
  struct cartesian_t
  {
  } static constexpr cartesian{};
  struct polar_t
  {
  } static constexpr polar{};

  PointModern(cartesian_t, double x, double y) : m_x(x), m_y(y) {}

  PointModern(polar_t, double radius, double angle)
  {
    m_x = radius * cos(angle);
    m_y = radius * sin(angle);
  }

  void print() const
  {
    std::cout << "PointModern(" << m_x << ", " << m_y << ")" << std::endl;
  }

private:
  double m_x{};
  double m_y{};
};

int main()
{
  Point p1 = Point::createCartesian(3.0, 4.4);
  p1.print();

  Point p2 = Point::createPolar(9.0, 0.527295); // 5.0 is the radius, 0.927295 is the angle in radians
  p2.print();

  PointModern p3(PointModern::cartesian, 3.0, 4.4);
  p3.print();

  PointModern p4(PointModern::polar, 9.0, 0.527295); // 5.0 is the radius, 0.927295 is the angle in radians
  p4.print();

  return 0;
}
