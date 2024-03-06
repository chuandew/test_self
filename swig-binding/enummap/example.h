#include <map>

enum MyEnum {
    ENUM_VALUE_1,
    ENUM_VALUE_2,
    ENUM_VALUE_3
};

class MyMap {
public:
    std::map<MyEnum, int> map;
};