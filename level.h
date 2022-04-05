#ifndef CLUB_LEVEL_H_
#define CLUB_LEVEL_H_

#include <cassert>
#include <cstdint>

#include "visitor.h"
#include "BadCharacteristic.h"
#include "typedefs.h"

#include <vector>

class ClubLevel {
 public:

  ClubLevel() = delete;
  ClubLevel(Days days, Money money);

  static constexpr int32_t kVisitorsCount = 10;
  static constexpr int32_t kCharacteristicCount = 4;

  Days GetDays() const {
    return days_;
  }

  Money GetMoneyPerDayCount() const {
    return money_;
  }

  std::vector<Visitor> GetAllVisitorsInDay(int day) const {
    return all_visitors_[day];
  }

  Visitor GetKthVisitorInDay(int visitor_number, int day) const {
    return all_visitors_[day][visitor_number];
  }

  BadCharacteristic GetBadCharacteristicsInDay(int day) const {
    return characteristic_[day];
  }

  std::vector<BadCharacteristic> GetAllBadCharacteristics() const {
    return characteristic_;
  };

 private:
  Days days_;
  Money money_;
  std::vector<std::vector<Visitor>> all_visitors_;
  std::vector<BadCharacteristic> characteristic_;
};

#endif  // CLUB_LEVEL_H_