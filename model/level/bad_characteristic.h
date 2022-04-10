#pragma once

#include "tools.h"
#include "guest.h"

#include <memory>
#include <vector>

struct BadCharacteristic {
 public:
  BadCharacteristic() = delete;
  explicit BadCharacteristic(int32_t characteristic_count);

 private:
  std::shared_ptr<Guest> guest_;
  std::vector<bool> is_valid_characteristic_;
};
