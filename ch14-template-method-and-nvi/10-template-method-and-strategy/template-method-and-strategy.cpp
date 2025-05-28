#include <iostream>

class CommunucationStrategy
{
public:
  virtual ~CommunucationStrategy() = default;

  virtual void open() const = 0;
  virtual void close() const = 0;
  virtual void send(int i) const = 0;
  virtual void send(double d) const = 0;
  virtual void send(long l) const = 0;
};

class Base
{
public:
  Base(int i) : m_i(i) {}

  void send(CommunucationStrategy *comm)
  {
    comm->open();
    sendData(comm);
    comm->close();
  }

protected:
  virtual void sendData(CommunucationStrategy *comm)
  {
    comm->send(m_i);
  }

private:
  int m_i{};
};

class Derived : public Base
{
public:
  Derived(int i, double d, long l) : Base(i), m_d(d), m_l(l) {}

protected:
  void sendData(CommunucationStrategy *comm) override
  {
    Base::sendData(comm);
    comm->send(m_d);
    comm->send(m_l);
  }

private:
  double m_d{};
  long m_l{};
};

class StandardOutputStrategy : public CommunucationStrategy
{
public:
  void open() const override
  {
    std::cout << "Opening standard output communication." << std::endl;
  }

  void close() const override
  {
    std::cout << "Closing standard output communication." << std::endl;
  }

  void send(int i) const override
  {
    std::cout << "Sending int: " << i << std::endl;
  }

  void send(double d) const override
  {
    std::cout << "Sending double: " << d << std::endl;
  }

  void send(long l) const override
  {
    std::cout << "Sending long: " << l << std::endl;
  }
};

int main()
{
  StandardOutputStrategy comm;
  Base base(42);
  base.send(&comm);

  Derived derived(11, 3.14, 1234567890);
  derived.send(&comm);

  return 0;
}
