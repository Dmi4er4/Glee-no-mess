#include "random.h"

int32_t Random::operator()(int32_t l, int32_t r) {
  int32_t size = r - l + 1;
  return l + abs(mersenne_twister_()) % size;
}
