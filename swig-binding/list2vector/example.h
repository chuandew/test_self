#include <iostream>
#include <string>
#include <vector>

struct Person {
  std::string name;
  int age;
};

void display(std::vector<Person> persons) {
  for (auto &p : persons) {
    std::cout << p.name << " " << p.age << std::endl;
  }
}