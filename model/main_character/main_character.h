#pragma once

#include "typedefs.h"

#include <cstdint>
#include <vector>

class MainCharacter {
 public:
  enum class InventoryItems {
    kItem1,
    kItem2,
    kItem3,
    kCount
  };

  MainCharacter();

  money_t GetMoney() const {
    return money_;
  }

 private:
  money_t money_{};
  std::vector<bool> inventory_{};
};
