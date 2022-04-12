#include "bad_characteristic.h"

BadCharacteristic::BadCharacteristic(int32_t characteristic_count) :
  guest_(new Guest) {
  is_valid_characteristic_.resize(characteristic_count);
  for (int i = 0; i < characteristic_count; ++i) {
    is_valid_characteristic_[i] = Random::RandomBool();
  }
}
