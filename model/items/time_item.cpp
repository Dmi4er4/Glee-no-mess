#include "time_item.h"

TimeItem::TimeItem() :
    Items("add_time") {
  auto file = std::get<QJsonDocument>(FileLoader::GetFile("items.json"));
  time_ = file["add_time"]["time"].toInt();
}

void TimeItem::TimeTrigger() {
  Model::AddTime(time_);
}
