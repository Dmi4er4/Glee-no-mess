#include "level.h"

Level::Level(const QString& file_name,
             const QString& level_name,
             const guests_t guests_count)
  : Level(FileLoader::GetFile<QJsonDocument>(file_name),
      level_name, guests_count) {
}

Level::Level(const QJsonDocument& source,
             const QString& level_name,
             const guests_t guests_count)
  : days_(source[level_name]["days"].toInt()),
      money_(source[level_name]["money"].toInt()),
      guests_count_(guests_count),
      characteristics_count_(source["characteristics_count"].toInt()) {
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
