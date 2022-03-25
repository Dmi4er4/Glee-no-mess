#include "main_character.h"

MainCharacter::MainCharacter() : money_(0) {
  inventory_.resize(static_cast<int32_t>(inventory_items::kCount), 0);
}
