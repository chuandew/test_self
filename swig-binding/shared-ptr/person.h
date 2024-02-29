#include <memory>
#include <string>

class Person {
public:
  std::string name;
  int age;

  Person(const std::string &name, int age) : name(name), age(age) {}
};

std::shared_ptr<Person> createPerson(const std::string &name, int age) {
  return std::make_shared<Person>(name, age);
}

std::string dumpString(std::shared_ptr<Person> person) {
  return "Name: " + person->name + ", Age: " + std::to_string(person->age);
}