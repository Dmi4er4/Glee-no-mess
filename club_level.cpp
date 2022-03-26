#include "club_level.h"

ClubLevel::ClubLevel(int32_t days, int64_t money) :
    days_(days),
    money_(money) {
  assert(days >= 0);
  all_visitors_.resize(days_);
  for (auto& day: all_visitors_) {
    day.resize(kVisitorsCount);
  }

  bad_characteristic_.resize(days_);
  is_valid_characteristic_.resize(days_);
  for (int i = 0; i < days_; ++i) {
    is_valid_characteristic_[i].resize(kCharacteristicCount);
    for (int j = 0; j < kCharacteristicCount; ++j) {
      is_valid_characteristic_[i][j] =
          std::uniform_int_distribution<int>(0, 1)(random_engine_);
    }
  }
}
