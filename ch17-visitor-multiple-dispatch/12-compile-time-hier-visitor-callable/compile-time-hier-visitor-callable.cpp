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
    visitor(thisPoint.m_x);
    visitor(thisPoint.m_y);
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
    visitor(thisLine.m_p1);
    visitor(thisLine.m_p2);
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
    visitor(thisIntersection.m_g1);
    visitor(thisIntersection.m_g2);
  }

private:
  G1 m_g1{};
  G2 m_g2{};
};

class BinarySerializeVisitor
{
public:
  explicit BinarySerializeVisitor(char *buffer, std::size_t size) : m_buffer(buffer), m_size(size) {}

  void operator()(double x)
  {
    if (m_size < sizeof(x))
      throw std::runtime_error("Buffer too small");

    memcpy(m_buffer, &x, sizeof(x));
    m_buffer += sizeof(x);
    m_size -= sizeof(x);
  }

  template <typename T>
  void operator()(const T &t)
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

  void operator()(double &x)
  {
    if (m_size < sizeof(x))
      throw std::runtime_error("Buffer too small");

    memcpy(&x, m_buffer, sizeof(x));
    m_buffer += sizeof(x);
    m_size -= sizeof(x);
  }

  template <typename T>
  void operator()(T &t)
  {
    T::accept(t, *this);
  }

private:
  const char *m_buffer{nullptr};
  std::size_t m_size{};
};

class StreamVisitor
{
public:
  explicit StreamVisitor(std::ostream &out) : m_out(out) {}

  void operator()(double x)
  {
    indent();
    m_out << x << std::endl;
  }

  template <typename T>
  void operator()(const T &t)
  {
    m_indent += 2;
    T::accept(t, *this);
    m_indent -= 2;
  }

private:
  void indent()
  {
    for (int i = 0; i < m_indent; ++i)
      m_out << " ";
  }
  std::ostream &m_out;
  int m_indent{};
};

template <typename V, typename... U>
void visitation(V &v, U &...u)
{
  (v(u), ...);
}

template <typename... U>
void serialize(char *buffer, size_t size, U &...u)
{
  BinarySerializeVisitor v(buffer, size);
  visitation(v, u...);
}

template <typename... U>
void deserialize(char *buffer, size_t size, U &...u)
{
  BinaryDeserializeVisitor v(buffer, size);
  visitation(v, u...);
}

template <typename T>
void write(const T &t, std::ostream &out = std::cout)
{
  StreamVisitor v(out);
  v(t);
}

int main()
{
  Line l1(Point(1, 2), Point(3, 4));
  Line l2(Point(5, 6), Point(7, 8));

  Intersection<Line, Line> x1(l1, l2);

  std::cout << "Line l1:\n";
  write(l1);
  std::cout << "Line l2:\n";
  write(l2);
  std::cout << "Intersection x:\n";
  write(x1);

  char buffer[1024];
  serialize(buffer, sizeof(buffer), l1, l2, x1);

  std::cout << "serialized, buffer: " << buffer << ", size: " << sizeof(buffer) << std::endl;

  Line l3, l4;
  Intersection<Line, Line> x2;
  deserialize(buffer, sizeof(buffer), l3, l4, x2);
  std::cout << "Line l3:\n";
  write(l3);
  std::cout << "Line l4:\n";
  write(l4);
  std::cout << "Intersection x2:\n";
  write(x2);
}
