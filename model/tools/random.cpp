#include "controller.h"
#include "random.h"

<<<<<<< Updated upstream
Random::Random() {}
=======
Random::Random() {
}
>>>>>>> Stashed changes

Random& Random::Instance() {
  static Random instance;
  return instance;
}
