#include <algorithm>
#include <future>
#include <iostream>
#include <ostream>
#include <thread>
#include <vector>

int main() {
  std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
  std::vector<std::future<void>> futures;
  for (int i = 0; i < 20; ++i) {
    auto fut = std::async(std::launch::deferred, [] {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << std::this_thread::get_id() << " \n";
    });
    futures.push_back(std::move(fut));
  }

  std::for_each(futures.begin(), futures.end(), [](std::future<void>& fut) { fut.wait(); });
  std::cout << std::endl;
}
