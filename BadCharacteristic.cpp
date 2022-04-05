#include "BadCharacteristic.h"

BadCharacteristic::BadCharacteristic(int32_t characteristic_count) {
  is_valid_characteristic_.resize(characteristic_count);
  for (int i = 0; i < characteristic_count; ++i) {
    is_valid_characteristic_[i] = rand(0, 1);
  }
}
