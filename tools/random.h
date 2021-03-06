#pragma once

#include <random>
#include <vector>

class Random {
 public:
  static Random& Instance() {
    static Random instance;
    return instance;
  }

  static int32_t RandomInt(int32_t l, int32_t r) {
    return std::uniform_int_distribution<int32_t>(l, r)(
        Instance().mersenne_twister_);
  }

  static bool RandomBool() {
    return std::uniform_int_distribution<int32_t>(0, 1)(
        Instance().mersenne_twister_);
  }

  template<class T>
  static T EnumChoose() {
    return static_cast<T>(
        std::uniform_int_distribution<int32_t>
            (0, static_cast<int>(T::kCount) - 1)(
            Instance().mersenne_twister_));
  }

  template<class T>
  static T VectorChoose(const std::vector<T>& vector) {
    auto index = RandomInt(0, vector.size() - 1);
    return vector[index];
  }

  std::mt19937 GetRandom() { return mersenne_twister_; }

 private:
  Random() {}

  std::mt19937 mersenne_twister_{std::random_device()()};
};
