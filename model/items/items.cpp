#include "items.h"

Items::Items(const QString& name) : name_(name) {
  auto file = std::get<QJsonDocument>(FileLoader::GetFile("items.json"));
  sprite_ = QPixmap(file["name"]["sprite"].toString());
}

void Items::MistakeTrigger() {
}

void Items::TimeTrigger() {
}
