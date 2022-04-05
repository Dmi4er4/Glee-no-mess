#pragma once

#include "bad_characteristic.h"
#include "../guest/guest.h"
#include "../tools/typedefs.h"

#include <QFile>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>

#include <cassert>
#include <cstdint>
#include <vector>

class Level {
 public:
  explicit Level(const QString& file_name);

  Days GetDays() const {
    return days_;
  }

  Money GetMoneyPerDayCount() const {
    return money_;
  }

  const std::vector<Guest>& GetAllVisitorsInDay(int day) const {
    return all_guests_[day];
  }

  Guest GetKthVisitorInDay(int visitor_number, int day) const {
    return all_guests_[day][visitor_number];
  }

  BadCharacteristic GetBadCharacteristicsInDay(int day) const {
    return characteristic_[day];
  }

  std::vector<BadCharacteristic> GetAllBadCharacteristics() const {
    return characteristic_;
  }

 private:
  void GenerateGuests();

  Visitors guests_count_;
  Characteristics characteristics_count_;
  Days days_;
  Money money_;
  std::vector<std::vector<Guest>> all_guests_;
  std::vector<BadCharacteristic> characteristic_;
};
