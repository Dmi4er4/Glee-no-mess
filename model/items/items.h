#pragma once

#include "file_loader.h"

#include <QPixmap>
#include <QString>

class Items {
 public:
  explicit Items(const QString& name);

  const QPixmap& GetSprite() const {
    return sprite_;
  }

  const QString& GetName() const {
    return name_;
  }

  virtual void MistakeTrigger();
  virtual void TimeTrigger();

 private:
  QString name_;
  QPixmap sprite_;
};
