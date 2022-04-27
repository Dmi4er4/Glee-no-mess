#include "view.h"
#include "controller.h"
#include "file_loader.h"

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
  setWindowState(Qt::WindowMaximized);
  const QPen transparent_pen(QColor(QColor::fromRgb(0, 0, 0, 0)));
  scene_->addLine(0, 0, width(), 0, transparent_pen);
  scene_->addLine(0, 0, 0, height(), transparent_pen);
  scene_->addLine(0, height(), width(), height(), transparent_pen);
  scene_->addLine(width(), 0, width(), height(), transparent_pen);
  permit_button_->setGeometry(5, 5, 130, 30);
  reject_button_->setGeometry(5, 40, 130, 30);
  // TODO(shandomruffle): remove kludge
  errors_->setGeometry(5, 75, 130, 23);
  SetErrorsCount(0);
  SetTimer();
  SetBackgroundImage(std::get<QPixmap>(FileLoader::GetFile(
      ":/club_level/background/background_frame_0.png")));

  graphics_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphics_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::SetErrorsCount(int value) {
  errors_->setText("Errors: " + QString::number(value));
}

void View::SetTimer() {
  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &View::ChangeFrame);
  timer->start(kFrameRate);
}

void View::SetBackgroundImage(const QPixmap& pixmap) {
  QPalette palette;
  palette.setBrush(QPalette::Base,
                   pixmap.scaled(size(), Qt::IgnoreAspectRatio));
  graphics_->setPalette(palette);
}

void View::ChangeFrame() {
  static constexpr uint8_t number_of_frames = 16;
  current_frame_++;
  current_frame_ %= number_of_frames;
  SetBackgroundImage(std::get<QPixmap>(FileLoader::GetFile(
      ":/club_level/background/background_frame_"
          + QString::number(current_frame_) + ".png")));
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::Instance().keyPressEvent(event);
}

View& View::Instance() {
  static View instance;
  return instance;
}
