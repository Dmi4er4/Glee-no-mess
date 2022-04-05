#include "level.h"

Level::Level(const QString& file_name) {
  QFile source(file_name);
  source.open(QIODevice::ReadOnly);
  QString s = source.readLine();
  QStringList list = s.split(" ");
  days_ = list[0].toUInt();
  money_ = list[1].toLongLong();
  visitors_count_ = list[2].toUInt();
  characteristic_count_ = list[3].toUInt();
  GenerateVisitors();
}

void Level::GenerateVisitors() {
  all_visitors_.resize(days_);
  for (auto& day: all_visitors_) {
    day.resize(visitors_count_);
  }
  characteristic_.resize(days_,
                         BadCharacteristic(characteristic_count_));
}
