#include "level.h"

Level::Level(const QString& file_name) {
  QJsonDocument source =
      std::get<QJsonDocument>(FileLoader::GetFile(file_name));
  days_ = source["days"].toInt();
  money_ = source["money"].toInt();
  guests_count_ = source["guests_count"].toInt();
  characteristics_count_ = source["characteristics_count"].toInt();
  GenerateGuests();
}

void Level::GenerateGuests() {
  all_guests_.resize(days_);
  characteristic_.resize(days_);
  for (int i = 0; i < days_; ++i) {
    all_guests_[i].resize(guests_count_);
    for (int j = 0; j < guests_count_; ++j) {
      all_guests_[i][j] = std::make_shared<Guest>();
    }

    characteristic_[i] =
        std::make_shared<BadCharacteristic>(characteristics_count_);
  }
}
