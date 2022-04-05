#include "level.h"

ClubLevel::ClubLevel(Days days, Money money) :
    days_(days),
    money_(money) {
  all_visitors_.resize(days_);
  for (auto& day: all_visitors_) {
    day.resize(kVisitorsCount);
  }
  characteristic_.resize(days,
                         BadCharacteristic(kCharacteristicCount));
}