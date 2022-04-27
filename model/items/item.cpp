#include "item.h"

Item::Item(const QString& name) : name_(name) {
  auto file = std::get<QJsonDocument>(FileLoader::GetFile("items.json"));
  sprite_ =
      std::get<QPixmap>(FileLoader::GetFile(file["name"]["sprite"].toString()));
}

void Item::MistakeTrigger() {
}

void Item::TimeTrigger() {
}
