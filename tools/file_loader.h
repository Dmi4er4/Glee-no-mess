#pragma once

#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QPixmap>
#include <QJsonDocument>

#include <map>

class FileLoader {
 public:
  FileLoader() = delete;
  static std::variant<QJsonDocument,
                      QPixmap> GetFile(const QString& file_name);

 private:
  static bool IsJsonFile(const QString& file_name);
  static bool IsImageFile(const QString& file_name);

  static QJsonDocument& GetJsonFile(const QString& file_name);
  static QPixmap& GetImageFile(const QString& file_name);
  static QString CastFileToString(const QString& file_name);

  static std::map<QString, QJsonDocument> container_json_;
  static std::map<QString, QPixmap> container_image_;
};
