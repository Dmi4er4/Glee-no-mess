#pragma once

#include "guest.h"

#include <QApplication>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QRect>
#include <QPushButton>
#include <QPalette>
#include <QWidget>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QFile>
#include <QTimer>

#include <deque>
#include <memory>

class Model;
class Controller;

class View : public QMainWindow {
 Q_OBJECT

 public:
  static View& Instance();
  static constexpr int kBlue = 0x0dc1fb;
  static constexpr int kYellow = 0xfdff73;
  static constexpr int kSquareSize = 80;
  static constexpr int kMargin = 5;
  static inline const QBrush kBluePolygonBrush = QBrush(QColor(kBlue));
  static inline const QBrush kYellowPolygonBrush = QBrush(QColor(kYellow));

  void keyPressEvent(QKeyEvent* event) override;

  void SetErrorsCount(int value);
  void SetTimer();
  void SetBackgroundImage(const QPixmap&);
  void ChangeFrame();

  auto GetPermitButton() { return permit_button_; }
  auto GetRejectButton() { return reject_button_; }
  auto GetScene() { return scene_; }

 private:
  View();
  QGraphicsScene* scene_;
  QPushButton* permit_button_;
  QPushButton* reject_button_;
  QGraphicsProxyWidget* proxy_widget_;
  QGraphicsView* graphics_;
  QLabel* errors_;

  uint8_t current_frame_;

  const uint8_t kFrameRate = 75;
};
