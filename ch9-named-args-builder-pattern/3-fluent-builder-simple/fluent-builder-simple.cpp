#include <iostream>
#include <cassert>

class Employee
{
public:
  friend std::ostream &operator<<(std::ostream &os, const Employee &employee)
  {
    return os << employee.m_prefix << " " << employee.m_firstName << " " << employee.m_lastName << " " << employee.m_suffix;
  }

private:
  Employee() = default;
  friend class EmployeeBuilder;

private:
  std::string m_prefix;
  std::string m_firstName;
  std::string m_lastName;
  std::string m_suffix;
};

class EmployeeBuilder
{
public:
  EmployeeBuilder &withPrefix(const std::string &prefix)
  {
    m_employee.m_prefix = prefix;
    return *this;
  }
  EmployeeBuilder &withFirstName(const std::string &firstName)
  {
    m_employee.m_firstName = firstName;
    return *this;
  }
  EmployeeBuilder &withLastName(const std::string &lastName)
  {
    m_employee.m_lastName = lastName;
    return *this;
  }
  EmployeeBuilder &withSuffix(const std::string &suffix)
  {
    m_employee.m_suffix = suffix;
    return *this;
  }

  operator Employee() const
  {
    assert(!m_employee.m_firstName.empty() && !m_employee.m_lastName.empty());
    return std::move(m_employee);
  }

private:
  Employee m_employee;
};

int main()
{
  Employee employee = EmployeeBuilder()
                          .withPrefix("Mr.")
                          .withFirstName("John")
                          .withLastName("Doe");
  std::cout << employee << '\n';
}
