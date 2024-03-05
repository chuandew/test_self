#include <iostream>
#include <string>
#include <vector>

struct Person {
  std::string name;
  int age;
};

void displayv1(std::vector<Person> persons) {
  std::cout << "========displayv1========" << std::endl;
  for (auto &p : persons) {
    std::cout << p.name << " " << p.age << std::endl;
  }
}

void displayv2(const std::vector<Person> &persons) {
  std::cout << "========displayv2========" << std::endl;
  for (auto &p : persons) {
    std::cout << p.name << " " << p.age << std::endl;
  }
}