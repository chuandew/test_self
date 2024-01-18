#include <chrono>
#include <future>
#include <iostream>
#include <optional>
#include <thread>
#include <vector>

//  g++ -Wall -Wextra -std=c++20 -pthread -O3 -DCONFIG_THREADS=false -DCONFIG_ITERATIONS=10000 -DCONFIG_POLICY=std::launch::deferred main.cpp -o main && ./main
//  g++ -Wall -Wextra -std=c++20 -pthread -O3 -DCONFIG_THREADS=false -DCONFIG_ITERATIONS=10000 -DCONFIG_POLICY=std::launch::deferred main.cpp -o main && ./main
//  g++ -Wall -Wextra -std=c++20 -pthread -O3 -DCONFIG_THREADS=false -DCONFIG_ITERATIONS=10000 -DCONFIG_POLICY=std::launch::deferred main.cpp -o main && ./main
//  g++ -Wall -Wextra -std=c++20 -pthread -O3 -DCONFIG_THREADS=false -DCONFIG_ITERATIONS=10000 -DCONFIG_POLICY=std::launch::async main.cpp -o main && ./main\nRunning benchmark:
//  g++ -Wall -Wextra -std=c++20 -pthread -O3 -DCONFIG_THREADS=true -DCONFIG_ITERATIONS=10000 -DCONFIG_POLICY=std::launch::async main.cpp -o main && ./main\nRunning benchmark:
//  g++ -Wall -Wextra -std=c++20 -pthread -O3 -DCONFIG_THREADS=true -DCONFIG_ITERATIONS=10000 -DCONFIG_POLICY=std::launch::async main.cpp -o main && ./main
//  g++ -Wall -Wextra -std=c++20 -pthread -O3 -DCONFIG_THREADS=true -DCONFIG_ITERATIONS=10000 -DCONFIG_POLICY=std::launch::async main.cpp -o main && ./main

__thread volatile int you_shall_not_optimize_this;

void work() {
  // This is the simplest way I can think of to prevent the compiler and
  // operating system from doing naughty things
  you_shall_not_optimize_this = 42;
}

[[gnu::noinline]] std::chrono::nanoseconds benchmark_one_thread(size_t count) {

  auto before = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < count; ++i) {
    work();
  }

  auto after = std::chrono::high_resolution_clock::now();

  return after - before;
}

[[gnu::noinline]] std::chrono::nanoseconds benchmark_threads(size_t count) {
  std::vector<std::optional<std::thread>> threads;
  threads.resize(count);

  auto before = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < count; ++i)
    threads[i] = std::thread{work};

  for (size_t i = 0; i < count; ++i)
    threads[i]->join();

  threads.clear();

  auto after = std::chrono::high_resolution_clock::now();

  return after - before;
}

[[gnu::noinline]] std::chrono::nanoseconds benchmark_async(size_t count,
                                                           std::launch policy) {
  std::vector<std::optional<std::future<void>>> results;
  results.resize(count);

  auto before = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < count; ++i)
    results[i] = std::async(policy, work);

  for (size_t i = 0; i < count; ++i)
    results[i]->wait();

  results.clear();

  auto after = std::chrono::high_resolution_clock::now();

  return after - before;
}

std::ostream &operator<<(std::ostream &stream, std::launch value) {
  if (value == std::launch::async)
    return stream << "std::launch::async";
  else if (value == std::launch::deferred)
    return stream << "std::launch::deferred";
  else
    return stream << "std::launch::unknown";
}

#define CONFIG_THREADS true
#define CONFIG_ITERATIONS 10000
#define CONFIG_POLICY std::launch::async

int main() {
  std::cout << "Running benchmark:\n"
            << "  threads?     " << std::boolalpha << CONFIG_THREADS << '\n'
            << "  iterations   " << CONFIG_ITERATIONS << '\n'
            << "  async policy " << CONFIG_POLICY << std::endl;

  std::chrono::nanoseconds duration;
  if (CONFIG_THREADS) {
    duration = benchmark_one_thread(CONFIG_ITERATIONS);
    // duration = benchmark_threads(CONFIG_ITERATIONS);
  } else {
    duration = benchmark_async(CONFIG_ITERATIONS, CONFIG_POLICY);
  }

  std::cout
      << "Completed in " << duration.count() << "ns ("
      << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
      << "ms)\n";
}