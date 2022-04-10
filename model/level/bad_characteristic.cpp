#include "bad_characteristic.h"

BadCharacteristic::BadCharacteristic(int32_t characteristic_count) :
  guest_(new Guest) {
  is_valid_characteristic_.resize(characteristic_count, Random::RandomBool());
}
