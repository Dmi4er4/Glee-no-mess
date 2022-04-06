#pragma once

#include "guest.h"
#include "mvc.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include <deque>

class Model;
class Controller;

class View : public QMainWindow {
  Q_OBJECT

 public:
  static constexpr int kWidth = 1000;
  static constexpr int kHeight = 800;

  static constexpr int kBlue = 0x0dc1fb;
  static constexpr int kYellow = 0xfdff73;
  static constexpr int kSquareSize = 80;
  static inline const QBrush kBluePolygonBrush = QBrush(QColor(kBlue));
  static inline const QBrush kYellowPolygonBrush = QBrush(QColor(kYellow));

  explicit View(Controller* controller);

  void keyPressEvent(QKeyEvent* event) override;
  void RenderActiveVisitor(Guest* current);
  void RenderQueue(const std::deque<Guest*>& queue);

  void SetErrorCount(int value);

 private:
  friend class Controller;
  QGraphicsScene* scene_;
  QPushButton* ok_;
  QPushButton* not_ok_;
  QGraphicsProxyWidget* proxy_widget_;
  QGraphicsView* graphics_;
  QLabel* errors_;

  Model* model_;
  Controller* controller_;
};
