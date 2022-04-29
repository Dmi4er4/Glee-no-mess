#include "view.h"
#include "controller.h"
#include "file_loader.h"

#include <QGuiApplication>

View::View()
    : scene_(new QGraphicsScene),
      permit_button_(new QPushButton("OK")),
      reject_button_(new QPushButton("NOT OK")),
      proxy_widget_(scene_->addWidget(permit_button_)),
      graphics_(new QGraphicsView(scene_)),
      errors_(new QLabel("Placeholder")) {
  qApp->setStyleSheet(FileLoader::CastFileToString(":main.qss"));
  auto screen_size = QGuiApplication::primaryScreen()->size();
  setFixedSize(screen_size);
  setWindowState(windowState() | Qt::WindowFullScreen);

  setCentralWidget(graphics_);

  scene_->addWidget(reject_button_);
  scene_->addWidget(errors_);
  scene_->addLine(0, 0, width(), height(), QPen(Qt::transparent));
  permit_button_->setGeometry(kMargin,
                              kMargin,
                              permit_button_->width(),
                              permit_button_->height());
  reject_button_->setGeometry(kMargin,
                              permit_button_->geometry().bottom() + kMargin,
                              reject_button_->width(),
                              reject_button_->height());
  errors_->setGeometry(kMargin,
                       reject_button_->geometry().bottom() + kMargin,
                       errors_->width(),
                       errors_->height());
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
