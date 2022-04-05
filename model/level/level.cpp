#include "level.h"

Level::Level(const QString& file_name) {
  QFile file(file_name);
  file.open(QIODevice::ReadOnly);
  QString s = file.readAll();
  file.close();

  QJsonDocument source = QJsonDocument::fromJson(s.toUtf8());
  days_ = source["days"].toInt();
  money_ = source["money"].toInt();
  guests_count_ = source["guests_count"].toInt();
  characteristics_count_ = source["characteristics_count"].toInt();
  GenerateGuests();
}

void Level::GenerateGuests() {
  all_guests_.resize(days_);
  for (auto& day : all_guests_) {
    day.resize(guests_count_);
  }
  characteristic_.resize(days_,
                         BadCharacteristic(characteristics_count_));
}
