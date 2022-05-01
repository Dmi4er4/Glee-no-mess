#include "item.h"

#include "file_loader.h"

Item::Item(const QString& name) : name_(name) {
  QJsonDocument file = FileLoader::GetFile<QJsonDocument>("items.json");
  sprite_ = FileLoader::GetFile<QPixmap>(file[name_]["sprite"].toString());
}
