#include "view.h"
#include "controller.h"
#include "model.h"

View::View()
    : scene_(new QGraphicsScene),
      permit_button_(new QPushButton("OK")),
      reject_button_(new QPushButton("NOT OK")),
      proxy_widget_(scene_->addWidget(permit_button_)),
      graphics_(new QGraphicsView(scene_)),
      errors_(new QLabel("Placeholder")) {
  scene_->addWidget(reject_button_);

  scene_->addWidget(errors_);

  setCentralWidget(graphics_);
  setFixedSize(kWidth, kHeight);
  const QPen transparent_pen(QColor(QColor::fromRgb(0, 0, 0, 0)));
  scene_->addLine(0, 0, kWidth, 0, transparent_pen);
  scene_->addLine(0, 0, 0, kHeight, transparent_pen);
  scene_->addLine(0, kHeight, kWidth, kHeight, transparent_pen);
  scene_->addLine(kWidth, 0, kWidth, kHeight, transparent_pen);
  permit_button_->setGeometry(5, 5, 130, 30);
  reject_button_->setGeometry(5, 40, 130, 30);
  errors_->setGeometry(5, 75, 130, 23);

  graphics_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphics_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::RenderActiveVisitor(Guest* current) {
  const QBrush& brush = current->IsMale()
      ? kBluePolygonBrush : kYellowPolygonBrush;
  Instance().scene_->addRect(5, kHeight / 2 - kSquareSize / 2,
                             kSquareSize, kSquareSize,
                             QPen(), brush);
}

void View::RenderQueue(const std::deque<Guest*>& queue) {
  for (int i = 0; i < queue.size(); ++i) {
    const QBrush& brush = queue[i]->IsMale()
        ? kBluePolygonBrush : kYellowPolygonBrush;
    Instance().scene_->addRect(kWidth - (kSquareSize + 5) * (queue.size() - i),
                               kHeight / 2 - kSquareSize / 2,
                               kSquareSize, kSquareSize,
                               QPen(), brush);
  }
}

void View::SetErrorCount(int value) {
  Instance().errors_->setText(
      QString("Errors: ") + std::to_string(value).c_str());
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::keyPressEvent(event);
}

View& View::Instance() {
  static View instance;
  return instance;
}
