#include <iostream>
#include <set>

int main() {
  std::set<std::string> s = {"a", "b", "c", "d"};
  std::set<std::string_view> t;
  for (const auto& str : s) {
    t.insert(str);
  }

  auto print = [&t] {
    std::cout << "t = { ";
    for (const auto& n : t) std::cout << n << ' ';
    std::cout << "}\n";
  };
  print();

  t.erase("a");
  print();

  std::cout << "Erase all odd numbers:\n";
  for (const auto & it : s) {
    // std::string_view sv{*it};
    // auto iter = t.find(sv);
    // if(iter!=t.end()){
    t.erase(it);
    // }
  }
  print();

  //   std::cout << "Erase 1, erased count: " << c.erase(1) << '\n';
  //   std::cout << "Erase 2, erased count: " << c.erase(2) << '\n';
  //   std::cout << "Erase 2, erased count: " << c.erase(2) << '\n';
  //   print();
}
