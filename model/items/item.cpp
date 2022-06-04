#include "item.h"

#include "file_loader.h"

Item::Item(const QString& name) : name_(name) {
  // TODO(Adamenko-Vladislav) add sprites of items
  // auto file = FileLoader::GetFile<QJsonDocument>("items.json");
  // sprite_ = FileLoader::GetFile<QPixmap>(file[name_]["sprite"].toString());
}
