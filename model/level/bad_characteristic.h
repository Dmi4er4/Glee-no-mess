#pragma once

#include <memory>
#include <vector>

#include "tools.h"
#include "guest.h"

struct BadCharacteristic {
 public:
  BadCharacteristic() = delete;
  explicit BadCharacteristic(int32_t characteristic_count);

 private:
  std::shared_ptr<Guest> guest_;
  std::vector<bool> is_valid_characteristic_;
};
