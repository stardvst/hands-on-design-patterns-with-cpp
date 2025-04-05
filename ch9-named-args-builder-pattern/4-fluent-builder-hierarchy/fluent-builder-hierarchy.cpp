#include <iostream>
#include <cassert>

class Employee
{
public:
  friend std::ostream &operator<<(std::ostream &os, const Employee &employee)
  {
    return os << employee.m_prefix << " " << employee.m_firstName << " " << employee.m_lastName << " " << employee.m_suffix << '\n'
              << "Job Title: " << employee.m_jobTitle << '\n'
              << "Address: " << employee.m_address << '\n'
              << "Awards: " << employee.m_awards.size();
  }

private:
  Employee() = default;
  friend class AbstractEmployeeBuilder;
  friend class EmployeeBuilder;
  friend class JobBuilder;
  friend class AddressBuilder;
  friend class AwardsBuilder;

private:
  std::string m_prefix;
  std::string m_firstName;
  std::string m_lastName;
  std::string m_suffix;
  std::string m_jobTitle;
  std::string m_address;
  std::vector<std::string> m_awards;
};

class JobBuilder;
class AddressBuilder;
class AwardsBuilder;

class AbstractEmployeeBuilder
{
public:
  explicit AbstractEmployeeBuilder(Employee &employee) : m_employee(employee) {}

  operator Employee() const
  {
    assert(!m_employee.m_firstName.empty() && !m_employee.m_lastName.empty());
    return std::move(m_employee);
  }

  JobBuilder Job();
  AddressBuilder Address();
  AwardsBuilder Awards();

protected:
  Employee &m_employee;
};

class EmployeeBuilder : public AbstractEmployeeBuilder
{
public:
  explicit EmployeeBuilder() : AbstractEmployeeBuilder(m_employee) {}

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

private:
  Employee m_employee;
};

class JobBuilder : public AbstractEmployeeBuilder
{
public:
  explicit JobBuilder(Employee &employee) : AbstractEmployeeBuilder(employee) {}
  JobBuilder &withJobTitle(const std::string &jobTitle)
  {
    m_employee.m_jobTitle = jobTitle;
    return *this;
  }
};

class AddressBuilder : public AbstractEmployeeBuilder
{
public:
  explicit AddressBuilder(Employee &employee) : AbstractEmployeeBuilder(employee) {}

  AddressBuilder &withAddress(const std::string &address)
  {
    m_employee.m_address = address;
    return *this;
  }
};

class AwardsBuilder : public AbstractEmployeeBuilder
{
public:
  explicit AwardsBuilder(Employee &employee) : AbstractEmployeeBuilder(employee) {}

  AwardsBuilder &withAwards(const std::string &awards)
  {
    m_employee.m_awards.push_back(awards);
    return *this;
  }
};

JobBuilder AbstractEmployeeBuilder::Job()
{
  return JobBuilder(m_employee);
}
AddressBuilder AbstractEmployeeBuilder::Address()
{
  return AddressBuilder(m_employee);
}
AwardsBuilder AbstractEmployeeBuilder::Awards()
{
  return AwardsBuilder(m_employee);
}

int main()
{
  Employee employee = EmployeeBuilder()
                          .withPrefix("Mr.")
                          .withFirstName("John")
                          .withLastName("Doe")
                          .Job()
                            .withJobTitle("Software Engineer")
                          .Address()
                            .withAddress("123 Main St.")
                          .Awards()
                            .withAwards("Award 1")
                            .withAwards("Award 2");
  std::cout << employee << '\n';
}
