#ifndef MAIN_CHARACTER_H_
#define MAIN_CHARACTER_H_

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

 private:
  Money money_{};
  std::vector<bool> inventory_{};
};

#endif  // MAIN_CHARACTER_H_
