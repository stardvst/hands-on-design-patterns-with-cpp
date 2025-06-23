#include <iostream>

// The reason for having the This argument is to be able to
// support both const and non-const operations easily

class Point
{
public:
  Point() = default;
  explicit Point(double x, double y) : m_x(x), m_y(y) {}

  template <typename This, typename Visitor>
  static void accept(This &thisPoint, Visitor &visitor)
  {
    visitor.visit(thisPoint.m_x);
    visitor.visit(thisPoint.m_y);
  }

private:
  double m_x{};
  double m_y{};
};

class Line
{
public:
  Line() = default;
  Line(Point p1, Point p2) : m_p1(std::move(p1)), m_p2(std::move(p2)) {}

  template <typename This, typename Visitor>
  static void accept(This &thisLine, Visitor &visitor)
  {
    Point::accept(thisLine.m_p1, visitor);
    Point::accept(thisLine.m_p2, visitor);
  }

private:
  Point m_p1{};
  Point m_p2{};
};

template <typename G1, typename G2>
class Intersection
{
public:
  Intersection() = default;
  Intersection(G1 g1, G2 g2) : m_g1(std::move(g1)), m_g2(std::move(g2)) {}

  template <typename This, typename Visitor>
  static void accept(This &thisIntersection, Visitor &visitor)
  {
    visitor.visit(thisIntersection.m_g1);
    visitor.visit(thisIntersection.m_g2);
  }

private:
  G1 m_g1{};
  G2 m_g2{};
};

class BinarySerializeVisitor
{
public:
  explicit BinarySerializeVisitor(char *buffer, std::size_t size) : m_buffer(buffer), m_size(size) {}

  void visit(double x)
  {
    std::cout << "\tVisiting double serializer" << std::endl;
    if (m_size < sizeof(x))
      throw std::runtime_error("Buffer too small");

    memcpy(m_buffer, &x, sizeof(x));
    m_buffer += sizeof(x);
    m_size -= sizeof(x);
    std::cout << "\t\tx: " << x << ", buffer: " << m_buffer << ", size: " << m_size << std::endl;
    std::cout << "\tDone visiting double serializer" << std::endl;
  }

  template <typename T>
  void visit(const T &t)
  {
    T::accept(t, *this);
  }

private:
  char *m_buffer{nullptr};
  std::size_t m_size{};
};

class BinaryDeserializeVisitor
{
public:
  explicit BinaryDeserializeVisitor(const char *buffer, std::size_t size) : m_buffer(buffer), m_size(size) {}

  void visit(double &x)
  {
    std::cout << "\tVisiting double deserializer" << std::endl;
    if (m_size < sizeof(x))
      throw std::runtime_error("Buffer too small");

    memcpy(&x, m_buffer, sizeof(x));
    m_buffer += sizeof(x);
    m_size -= sizeof(x);
    std::cout << "\t\tx: " << x << ", buffer: " << m_buffer << ", size: " << m_size << std::endl;
    std::cout << "\tDone visiting double deserializer" << std::endl;
  }

  template <typename T>
  void visit(T &t)
  {
    T::accept(t, *this);
  }

private:
  const char *m_buffer{nullptr};
  std::size_t m_size{};
};

int main()
{
  Line l1(Point(1, 2), Point(3, 4));
  Line l2(Point(5, 6), Point(7, 8));

  Intersection<Line, Line> x1(l1, l2);

  char buffer[100];
  BinarySerializeVisitor serializer(buffer, sizeof(buffer));
  serializer.visit(l1);
  serializer.visit(x1);

  BinaryDeserializeVisitor deserializer(buffer, sizeof(buffer));
  Point p;
  Line l;
  Intersection<Point, Line> x;
  deserializer.visit(l);
  deserializer.visit(x);
}
