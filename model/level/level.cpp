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
  for (int i = 0; i < days_; ++i) {
    all_guests_[i] =
        std::vector<std::shared_ptr<Guest>>(guests_count_,
                                            std::shared_ptr<Guest>(new Guest));
  }
  characteristic_.resize(days_,
                         std::make_shared<BadCharacteristic>(BadCharacteristic(
                             characteristics_count_)));
}
