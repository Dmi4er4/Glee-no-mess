#include "file_loader.h"

std::unordered_map<QString, QJsonDocument> FileLoader::container_json_ = {};
std::unordered_map<QString, QPixmap> FileLoader::container_image_ = {};

std::variant<QJsonDocument,
             QPixmap> FileLoader::GetFile(const QString& file_name) {
  if (IsJsonFile(file_name)) {
    return GetJsonFile(file_name);
  } else if (IsImageFile(file_name)) {
    return GetImageFile(file_name);
  } else {
    throw std::invalid_argument("Unknown file format");
  }
}

bool FileLoader::IsJsonFile(const QString& file_name) {
  return endsWith(file_name, ".json");
}

bool FileLoader::IsImageFile(const QString& file_name) {
  return endsWith(file_name, ".jpeg", ".png", ".jpg");
}

QJsonDocument& FileLoader::GetJsonFile(const QString& file_name) {
  if (!container_json_.count(file_name)) {
    container_json_[file_name] =
        QJsonDocument::fromJson(CastFileToString(file_name).toUtf8());
  }
  return container_json_[file_name];
}

QPixmap& FileLoader::GetImageFile(const QString& file_name) {
  if (!container_image_.count(file_name)) {
    container_image_[file_name] = QPixmap(file_name);
  }
  return container_image_[file_name];
}

QString FileLoader::CastFileToString(const QString& file_name) {
  QFile file(file_name);
  file.open(QIODevice::ReadOnly);
  QString source;
  if (file.isOpen()) {
    source = file.readAll();
  }
  return source;
}
