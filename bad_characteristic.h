#ifndef BAD_CHARACTERISTIC_H_
#define BAD_CHARACTERISTIC_H_

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

#endif  // BAD_CHARACTERISTIC_H_
