#include "item.h"

#include "file_loader.h"

Item::Item(const QString& name) : name_(name) {
  auto file = std::get<QJsonDocument>(FileLoader::GetFile("items.json"));
  sprite_ =
      std::get<QPixmap>(FileLoader::GetFile(file[name_]["sprite"].toString()));
}
