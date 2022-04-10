#pragma once

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPixmap>
#include <QString>

#include <map>

class FileController {
 public:
  static FileController& GetInstance();

  static std::variant<QJsonDocument,
                      QPixmap> GetFile(const QString& file_name) {
    if (IsJsonFile(file_name)) {
      return GetJsonFile(file_name);
    } else if (IsImageFile(file_name)) {
      return GetImageFile(file_name);
    }
  }

 private:
  FileController() = default;

  static bool IsJsonFile(const QString& file_name);
  static bool IsImageFile(const QString& file_name);

  static QJsonDocument& GetJsonFile(const QString& file_name);
  static QPixmap& GetImageFile(const QString& file_name);
  static QString GetString(const QString& file_name);

  static std::map<QString, QJsonDocument> container_json_;
  static std::map<QString, QPixmap> container_image_;
};
