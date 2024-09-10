
#include <cstdint>
#include <iostream>
#include <variant> // Add this line

struct ScalarVar {
  std::variant<bool, int32_t, int64_t, float, double, std::string> data;
};

struct ScalarUnion {
  union {
    bool bool_data;
    int32_t int_data;
    int64_t long_data;
    float float_data;
    double double_data;
    std::string string_data;
    std::string bytes_data;
  };
};

int main(int argc, char* argv[]) {
  std::cout << "v0:" << argv[0] << std::endl;
  // print sizeof ScalarVar and ScalarUnion
  std::cout << "sizeof ScalarVar: " << sizeof(ScalarVar) << std::endl;
  std::cout << "sizeof ScalarUnion: " << sizeof(ScalarUnion) << std::endl;
  return 0;
}
