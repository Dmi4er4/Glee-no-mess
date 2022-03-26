#ifndef CLUB_LEVEL_H_
#define CLUB_LEVEL_H_

#include <cassert>
#include <cstdint>

#include "visitor.h"

#include <random>
#include <vector>

class ClubLevel {
 public:

  ClubLevel() = delete;
  ClubLevel(int32_t days, int64_t money);

  const int32_t kVisitorsCount = 10;
  const int32_t kCharacteristicCount = 4;

  int32_t GetdaysCount() const {
    return days_;
  }

  int64_t GetMoneyPerDayCount() const {
    return money_;
  }

  std::vector<Visitor> GetAllVisitorsInDay(int day) const {
    assert(day >= 0 && day < days_);
    return all_visitors_[day];
  }

  Visitor GetKthVisitorInDay(int k, int day) const {
    assert(k >= 0 && k < kVisitorsCount);
    assert(day >= 0 && day < days_);
    return all_visitors_[day][k];
  }

  std::pair<Visitor,
            std::vector<bool>> GetBadCharacteristicsInDay(int day) const {
    assert(day >= 0 && day < days_);
    return {bad_characteristic_[day], is_valid_characteristic_[day]};
  }

  std::pair<std::vector<Visitor>,
    std::vector<std::vector<bool>>> GetAllBadCharacteristics() const {
    return {bad_characteristic_, is_valid_characteristic_};
  };

 private:
  std::default_random_engine random_engine_;

  int32_t days_;
  int64_t money_;

  std::vector<std::vector<Visitor>> all_visitors_;

  std::vector<Visitor> bad_characteristic_;
  std::vector<std::vector<bool>> is_valid_characteristic_;
};

#endif  // CLUB_LEVEL_H_
