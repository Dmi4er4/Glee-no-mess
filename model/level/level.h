#pragma once

#include "bad_characteristic.h"
#include "guest.h"
#include "typedefs.h"

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

  days_t GetDays() const {
    return days_;
  }

  money_t GetMoneyPerDayCount() const {
    return money_;
  }

  const std::vector<Guest>& GetAllVisitorsInDay(int day) const {
    return all_guests_[day];
  }

  const Guest& GetKthVisitorInDay(int visitor_number, int day) const {
    return all_guests_[day][visitor_number];
  }

  const BadCharacteristic& GetBadCharacteristicsInDay(int day) const {
    return characteristic_[day];
  }

  const std::vector<BadCharacteristic>& GetAllBadCharacteristics() const {
    return characteristic_;
  }

 private:
  void GenerateGuests();

  guests_t guests_count_;
  characteristics_t characteristics_count_;
  days_t days_;
  money_t money_;
  std::vector<std::vector<Guest>> all_guests_;
  std::vector<BadCharacteristic> characteristic_;
};
