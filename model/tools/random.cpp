#include "mvc.h"
#include "random.h"

Random::Random() {
  Controller::Init();
}

Random& Random::Instance() {
  static Random instance;
  return instance;
}
