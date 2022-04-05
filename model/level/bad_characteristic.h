#pragma once

#include "../tools/tools.h"
#include "../guest/guest.h"

#include <vector>

struct BadCharacteristic {
 public:
  BadCharacteristic() = delete;
  explicit BadCharacteristic(int32_t characteristic_count);

 private:
  Random rand;
  Guest visitor_;
  std::vector<bool> is_valid_characteristic_;
};
