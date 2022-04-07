#pragma once

#include <random>

class Random {
 public:
  int32_t operator()(int32_t l, int32_t r);
 private:
  std::mt19937 mersenne_twister_{std::random_device()()};
};