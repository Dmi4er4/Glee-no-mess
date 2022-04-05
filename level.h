#ifndef LEVEL_H_
#define LEVEL_H_

#include "bad_characteristic.h"
#include "visitor.h"
#include "typedefs.h"

#include <QFile>
#include <QString>
#include <QStringList>

#include <cassert>
#include <cstdint>
#include <vector>

/*
 * days_
 * money_
 * count_guests_
 * count_characteristic_
 *
 * :/club_level.txt
 */

class Level {
 public:
  explicit Level(const QString& file_name);

  Days GetDays() const {
    return days_;
  }

  Money GetMoneyPerDayCount() const {
    return money_;
  }

  const std::vector<Visitor>& GetAllVisitorsInDay(int day) const {
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
  }

 private:
  void GenerateVisitors();

  Visitors visitors_count_;
  Characteristics characteristic_count_;
  Days days_;
  Money money_;
  std::vector<std::vector<Visitor>> all_visitors_;
  std::vector<BadCharacteristic> characteristic_;
};

#endif  // LEVEL_H_
