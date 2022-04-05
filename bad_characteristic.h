#ifndef BADCHARACTERISTIC_H_
#define BADCHARACTERISTIC_H_

#include "tools.h"
#include "visitor.h"

#include <vector>

struct BadCharacteristic {
 public:
  BadCharacteristic() = delete;
  explicit BadCharacteristic(int32_t characteristic_count);

 private:
  Random rand;
  Visitor visitor_;
  std::vector<bool> is_valid_characteristic_;
};

#endif  // BADCHARACTERISTIC_H_
