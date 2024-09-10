#include <iostream>
#include <ratio>
#include <set>

using namespace std;

void TestOperator() {
  string_view sv1{"ABA"};

  string_view sv2{"ABAD"};
  string_view sv3{"ABAD"};

  string_view sv4{"ABACE"};

  std::string s1{"ABAD"};

  bool result = sv2 > sv1; // true
  std::cout << "sv2 > sv1 result: " << (result ? "true" : "false") << std::endl;

  result = sv3 > sv2; // false
  std::cout << "sv3 > sv2 result: " << (result ? "true" : "false") << std::endl;

  result = sv3 != sv1; // true
  std::cout << "sv3 != sv1 result: " << (result ? "true" : "false")
            << std::endl;

  result = sv2 != sv3; // false
  std::cout << "sv2 != sv3 result: " << (result ? "true" : "false")
            << std::endl;

  result = sv2 != s1; // false
  std::cout << "sv2 != s1 result: " << (result ? "true" : "false") << std::endl;

  result = sv2 == s1; // true
  std::cout << "sv2 == s1 result: " << (result ? "true" : "false") << std::endl;

  result = sv4 < sv3; // true because `C` < `D`
  std::cout << "sv4 < sv3 result: " << (result ? "true" : "false") << std::endl;
}

int main() {
  std::set<std::string> s = {"a", "b", "c", "d"};
  std::set<std::string_view> t;
  for (const auto &str : s) {
    t.insert(str);
  }

  auto print = [&t] {
    std::cout << "t = { ";
    for (const auto &n : t)
      std::cout << n << ' ';
    std::cout << "}\n";
  };
  print();

  t.erase("a");
  print();

  std::cout << "Erase all odd numbers:\n";
  for (const auto &it : s) {
    // std::string_view sv{*it};
    // auto iter = t.find(sv);
    // if(iter!=t.end()){
    t.erase(it);
    // }
  }
  print();

  // std::cout << "Erase 1, erased count: " << c.erase(1) << '\n';
  // std::cout << "Erase 2, erased count: " << c.erase(2) << '\n';
  // std::cout << "Erase 2, erased count: " << c.erase(2) << '\n';
  // print();

  TestOperator();
}
