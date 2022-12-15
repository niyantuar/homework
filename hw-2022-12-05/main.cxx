#include <iostream>
#include <tuple>
#include <unordered_map>
#include <functional>
#include <cstddef>
#include <pthread.h>
#include <vector>
#include <algorithm>

std::unordered_map<std::string, std::function<int(int, int)>> functions{
  {"s", std::plus<int>()},
  {"m", std::multiplies<int>()},
  {"ss", [](int left, int right){return left*left + right*right;}}
};

struct args_t {
  int left;
  int right;
  std::string function;
};

void* compute(void* input) {
  auto [left, right, function] = *static_cast<args_t*>(input);
  return new int{functions[function](left, right)};
}

void print(std::pair<args_t, int*> expression) {
  auto [left, right, function] = expression.first;
  std::cout << left << " " << function << " " << right << " = " << *expression.second << std::endl;
}

int main() {
    std::size_t N;
    std::cin >> N;
    std::vector<pthread_t> threads(N);
    std::vector<std::pair<args_t, int*>> results(N);
    for (std::size_t i{}; i < N; ++i) {
        int left, right;
        std::string function;
        std::cin >> left >> right >> function;
        results[i].first = {left, right, function};
        void* args = &results[i].first;
        if (pthread_create(
          &threads[i], nullptr, compute, args
        )) {
          std::cerr << "thread " << i << " wasn't made" << std::endl;
          return -1;
        }
    }

    for (std::size_t i{}; i < N; ++i) {
      void* result{};
      pthread_join(threads[i], &result);
      results[i].second = static_cast<int*>(result);
    }

    for (auto result : results) {
      print(result);
    }

    for (auto el : results) {
      delete el.second;
    }
}

