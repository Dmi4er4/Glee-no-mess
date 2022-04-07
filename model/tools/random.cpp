#include "controller.h"
#include "random.h"

Random::Random() {}

Random& Random::Instance() {
  static Random instance;
  return instance;
}
