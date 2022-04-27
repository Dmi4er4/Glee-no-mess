#include "view.h"
#include "controller.h"
#include "model.h"

#include <iostream>

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
  // TODO(shandomruffle): remove kludge
  errors_->setGeometry(5, 75, 130, 23);
  SetErrorsCount(0);
  SetTimer();
  SetBackgroundImage(":/club_level/background/background_frame_1.png");

  graphics_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphics_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::SetErrorsCount(int value) {
  errors_->setText(QString("Errors: ") + std::to_string(value).c_str());
}

void View::SetTimer() {
  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this]() {
    ChangeFrame();
  });
  timer->start(75);
}

void View::SetBackgroundImage(const QString& path) {
  QPixmap path1(path);
  path1 = path1.scaled(this->size(), Qt::IgnoreAspectRatio);
  QPalette palette;
  palette.setBrush(QPalette::Base, path1);
  graphics_->setPalette(palette);
}
void View::ChangeFrame() {
  current_frame_++;
  current_frame_ %= 16;
  SetBackgroundImage(":/club_level/background/background_frame_"
                         + QString::number(current_frame_ + 1) + ".png");
}
void View::keyPressEvent(QKeyEvent* event) {
  Controller::Instance().keyPressEvent(event);
}

View& View::Instance() {
  static View instance;
  return instance;
}
