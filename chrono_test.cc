#include <iostream>
#include <chrono>
/*
int main ()
{
    using namespace std::chrono;
    // Get current time with precision of milliseconds
    auto now = time_point_cast<milliseconds>(system_clock::now());
    // sys_milliseconds is type time_point<system_clock, milliseconds>
    using sys_milliseconds = decltype(now);
    // Convert time_point to signed integral type
    auto integral_duration = now.time_since_epoch().count();
    // Convert signed integral type to time_point
    sys_milliseconds dt{milliseconds{integral_duration}};
    // test
    if (dt != now)
        std::cout << "Failure." << std::endl;
    else
        std::cout << "Success." << std::endl;
}
*/
int main ()
{
    using namespace std::chrono;
    auto now = steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() << "\n";

    auto now_ms = time_point_cast<milliseconds>(now);
    std::cout << now_ms.count() << "\n";

    auto value = now_ms.time_since_epoch();
    std::cout << value.count() << "\n";


    uint64_t duration = value.count();

    std::cout << "dur." << duration << std::endl;

    uint64_t next = duration_cast<milliseconds>(value + std::chrono::milliseconds(1000)).count();

    std::cout << "next." << next << std::endl;


}
