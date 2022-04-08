#pragma once

#include "guest.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QGraphicsView>

#include <deque>

class Model;
class Controller;

class View : public QMainWindow {
  Q_OBJECT

 public:
  static View& Instance();

  static constexpr int kWidth = 1000;
  static constexpr int kHeight = 800;

  static constexpr int kBlue = 0x0dc1fb;
  static constexpr int kYellow = 0xfdff73;
  static constexpr int kSquareSize = 80;
  static inline const QBrush kBluePolygonBrush = QBrush(QColor(kBlue));
  static inline const QBrush kYellowPolygonBrush = QBrush(QColor(kYellow));

  void keyPressEvent(QKeyEvent* event) override;

  static void RenderActiveVisitor(Guest* current);
  static void RenderQueue(const std::deque<std::unique_ptr<Guest>>& queue);

  static void SetErrorCount(int value);

  static auto GetPermitButton() { return Instance().permit_button_; }
  static auto GetRejectButton() { return Instance().reject_button_; }

 private:
  View();
  QGraphicsScene* scene_;
  QPushButton* permit_button_;
  QPushButton* reject_button_;
  QGraphicsProxyWidget* proxy_widget_;
  QGraphicsView* graphics_;
  QLabel* errors_;
};
