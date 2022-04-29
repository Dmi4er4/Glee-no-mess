#include "time_item.h"

#include "all_items.h"
#include "file_loader.h"

TimeItem::TimeItem() :
    Item(kAddTime) {
  auto file = std::get<QJsonDocument>(FileLoader::GetFile("items.json"));
  time_ = file[kAddTime]["time"].toInt();
}

void TimeItem::TimeTrigger() {
  Model::Instance().AddTime(time_);
}
