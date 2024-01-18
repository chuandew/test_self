#include <vector>
#include <iostream>
#include <thread>

int main() {
  int thread_nums = 100000;
  std::vector<std::thread> threads;
      for (int i = 0; i < thread_nums; ++i) {
      std::thread t([=](){
          std::cerr << "Thread " << i << " start" << "\n";

          std::cerr << "Thread " << i << " end" << "\n";
      });

      threads.push_back(std::move(t));
    }

    // join all thread
    for (auto& t : threads) {
      t.join();
    }

}
