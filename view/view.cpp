#include "view.h"

View::View(Controller* controller)
    : scene_(new QGraphicsScene),
      ok_(new QPushButton("OK")),
      not_ok_(new QPushButton("NOT OK")),
      proxy_widget_(scene_->addWidget(ok_)),
      graphics_(new QGraphicsView(scene_)),
      errors_(new QLabel("Placeholder")),
      controller_(controller) {
  scene_->addWidget(not_ok_);

  scene_->addWidget(errors_);

  setCentralWidget(graphics_);
  setFixedSize(kWidth, kHeight);
  const QPen transparent_pen(QColor(QColor::fromRgb(0, 0, 0, 0)));
  scene_->addLine(0, 0, kWidth, 0, transparent_pen);
  scene_->addLine(0, 0, 0, kHeight, transparent_pen);
  scene_->addLine(0, kHeight, kWidth, kHeight, transparent_pen);
  scene_->addLine(kWidth, 0, kWidth, kHeight, transparent_pen);
  ok_->setGeometry(5, 5, 130, 30);
  not_ok_->setGeometry(5, 40, 130, 30);
  errors_->setGeometry(5, 75, 130, 23);

  graphics_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphics_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::RenderActiveVisitor(Guest* current) {
  const QBrush& brush = current->IsMale()
      ? kBluePolygonBrush : kYellowPolygonBrush;
  scene_->addRect(5, kHeight / 2 - kSquareSize / 2,
                  kSquareSize, kSquareSize,
                  QPen(), brush);
}

void View::RenderQueue(const std::deque<Guest*>& queue) {
  for (int i = 0; i < queue.size(); ++i) {
    const QBrush& brush = queue[i]->IsMale()
        ? kBluePolygonBrush : kYellowPolygonBrush;
    scene_->addRect(kWidth - (kSquareSize + 5) * (queue.size() - i),
                    kHeight / 2 - kSquareSize / 2,
                    kSquareSize, kSquareSize,
                    QPen(), brush);
  }
}

void View::SetErrorCount(int value) {
  errors_->setText(QString("Errors: ") + std::to_string(value).c_str());
}

void View::keyPressEvent(QKeyEvent* event) {
  controller_->keyPressEvent(event);
}
