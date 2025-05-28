#include <iostream>

class FileWriter
{
public:
  void write(const std::string &data) const
  {
    preScript(data);

    // ... write data to file

    postScript(data);
  }

  virtual void preScript(const std::string &data) const {}
  virtual void postScript(const std::string &data) const {}
};

class LoggingFileWriter : public FileWriter
{
public:
  void preScript(const std::string &data) const override
  {
    std::cout << "Writing \"" << data << "\" to file\n";
  }

  void postScript(const std::string &data) const override
  {
    std::cout << "File written\n";
  }
};

int main()
{
  std::unique_ptr<FileWriter> writer = std::make_unique<LoggingFileWriter>();
  writer->write("Hello, World!");

  return 0;
}
