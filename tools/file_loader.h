#pragma once

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QString>

#include <unordered_map>

class FileLoader {
 public:
  FileLoader() = delete;

  template<typename T>
  static T GetFile(const QString& file_name) {
    return std::get<T>(LoadFile(file_name));
  }

  static QString CastFileToString(const QString& file_name);

 private:
  static std::variant<QJsonDocument,
                      QPixmap> LoadFile(const QString& file_name);

  static bool IsJsonFile(const QString& file_name);

  static bool IsImageFile(const QString& file_name);
  static QJsonDocument& GetJsonFile(const QString& file_name);
  static QPixmap& GetImageFile(const QString& file_name);

  static bool endsWith(const QString& filename, const QString& suffix) {
    return filename.endsWith(suffix);
  }

  template<typename... Args>
  static bool endsWith(const QString& filename,
                       const QString& suffix,
                       const Args& ... args) {
    return filename.endsWith(suffix) || endsWith(filename, args...);
  }

  static std::unordered_map<QString, QJsonDocument> container_json_;
  static std::unordered_map<QString, QPixmap> container_image_;
};
