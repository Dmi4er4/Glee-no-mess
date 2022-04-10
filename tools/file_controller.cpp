#include "file_controller.h"

std::map<QString, QJsonDocument> FileController::container_json_ = {};
std::map<QString, QPixmap> FileController::container_image_ = {};

FileController& FileController::GetInstance() {
  static FileController instance;
  return instance;
}

QJsonDocument& FileController::GetJsonFile(const QString& file_name) {
  if (!container_json_.count(file_name)) {
    container_json_[file_name] =
        QJsonDocument::fromJson(GetString(file_name).toUtf8());
  }
  return container_json_[file_name];
}

QString FileController::GetString(const QString& file_name) {
  QFile file(file_name);
  file.open(QIODevice::ReadOnly);
  QString source;
  if (file.isOpen()) {
    source = file.readAll();
  }
  return source;
}

bool FileController::IsJsonFile(const QString& file_name) {
  return file_name.endsWith(".json");
}

bool FileController::IsImageFile(const QString& file_name) {
  return file_name.endsWith(".jpg") ||
      file_name.endsWith(".jpeg") ||
      file_name.endsWith(".png");
}

QPixmap& FileController::GetImageFile(const QString& file_name) {
  if (!container_image_.count(file_name)) {
    container_image_[file_name] = QPixmap(file_name);
  }
  return container_image_[file_name];
}
