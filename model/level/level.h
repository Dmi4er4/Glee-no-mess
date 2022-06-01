#pragma once

#include <QFile>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "bad_characteristic.h"
#include "file_loader.h"
#include "guest.h"
#include "typedefs.h"

class Level {
 public:
  explicit Level(const QString& file_name,
                 const QString& level_name,
                 const guests_t guests_count);

  days_t GetDays() const {
    return days_;
  }

  money_t GetMoneyPerDayCount() const {
    return money_;
  }

  const std::vector<std::shared_ptr<Guest>>& GetAllGuestsInDay(int day) const {
    return all_guests_[day - 1];
  }

  const std::shared_ptr<Guest>& GetKthGuestInDay(int visitor_number,
                                                 int day) const {
    return all_guests_[day - 1][visitor_number];
  }

  std::shared_ptr<BadCharacteristic> GetBadCharacteristicsInDay(int day) const {
    return characteristic_[day - 1];
  }

  const std::vector<std::shared_ptr<BadCharacteristic>>&
    GetAllBadCharacteristics() const {
      return characteristic_;
  }

 private:
  Level(const QJsonDocument& source,
        const QString& level_name,
        const guests_t guests_count);

  void GenerateGuests();

  const guests_t guests_count_;
  const characteristics_t characteristics_count_;
  const days_t days_;
  const money_t money_;
  std::vector<std::vector<std::shared_ptr<Guest>>> all_guests_;
  std::vector<std::shared_ptr<BadCharacteristic>> characteristic_;
};
