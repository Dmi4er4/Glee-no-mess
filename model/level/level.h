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

  GuestsCount guests_count_;
  CharacteristicsCount characteristics_count_;
  Days days_;
  Money money_;
  std::vector<std::vector<Guest>> all_guests_;
  std::vector<BadCharacteristic> characteristic_;
};
