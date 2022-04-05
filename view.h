#ifndef VIEW_H_
#define VIEW_H_

#include "visitor.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <QLabel>

#include <deque>

class Model;
class Controller;

class View : public QMainWindow {
 Q_OBJECT

 public:
  explicit View(Controller* controller);

  void keyPressEvent(QKeyEvent* event) override;

  void RenderActiveVisitor(Visitor* current);
  void RenderQueue(const std::deque<Visitor*>& queue);
  void SetErrorCount(int value);

  QGraphicsScene* scene_;
  QPushButton* ok_;
  QPushButton* not_ok_;
  QGraphicsProxyWidget* proxy_widget_;
  QGraphicsView* graphics_;
  QLabel* errors_;

  static constexpr int kWidth = 1000, kHeight = 800;
  static constexpr int kBlue = 0x0dc1fb;
  static constexpr int kYellow = 0xfdff73;
  static constexpr int kSquareSize = 80;
  static inline const QBrush kBluePolygonBrush = QBrush(QColor(kBlue));
  static inline const QBrush kYellowPolygonBrush = QBrush(QColor(kYellow));

 private:
  friend class Controller;

  Model* model_;
  Controller* controller_;
};

#endif  // VIEW_H_
