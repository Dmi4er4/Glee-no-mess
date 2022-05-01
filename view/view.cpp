#include "view.h"

#include "controller.h"
#include "file_loader.h"
#include "settings.h"

#include <QGuiApplication>

View::View() :
    // game
    game_scene_(new QGraphicsScene),
    permit_button_(new QPushButton("OK")),
    reject_button_(new QPushButton("NOT OK")),
    game_graphics_(new QGraphicsView(game_scene_)),
    errors_(new QLabel("Placeholder")),
    proxy_permit_(game_scene_->addWidget(permit_button_)),
    proxy_reject_(game_scene_->addWidget(reject_button_)),
    proxy_errors_(game_scene_->addWidget(errors_)),
    // menu
    menu_scene_(new QGraphicsScene),
    menu_graphics_(new QGraphicsView(menu_scene_)),
    start_game_(new QPushButton("Play")),
    open_settings_(new QPushButton("Settings")),
    proxy_stat_game_(menu_scene_->addWidget(start_game_)),
    proxy_open_settings_(menu_scene_->addWidget(open_settings_)),
    // settings
    settings_scene_(new QGraphicsScene),
    settings_graphics_(new QGraphicsView(settings_scene_)),
    exit_settings_(new QPushButton("Main menu")),
    complexity_(new QPushButton),
    exit_shortcut_(new QLabel("Exit shortcut: ")),
    sound_(new QPushButton),
    default_settings_(new QPushButton("Set default settings")),
    proxy_exit_settings_(settings_scene_->addWidget(exit_settings_)),
    proxy_complexity_(settings_scene_->addWidget(complexity_)),
    proxy_exit_shortcut_(settings_scene_->addWidget(exit_shortcut_)),
    proxy_sound_(settings_scene_->addWidget(sound_)),
    proxy_default_settings_(settings_scene_->addWidget(default_settings_)) {

  // full screen mod
  auto screen_size = QGuiApplication::primaryScreen()->size();
  setFixedSize(screen_size);
  setWindowState(windowState() | Qt::WindowFullScreen);

  CustomGameScene();
  CustomMainMenu();
  CustomSettings();

  ShowMainMenu();
}

void View::SetErrorsCount(int value) {
  errors_->setText("Errors: " + QString::number(value));
}

void View::SetTimer() {
  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &View::ChangeFrame);
  timer->start(kFrameRate);
}

void View::SetBackgroundImage(QGraphicsView* graphics_view,
                              const QPixmap& pixmap) {
  QPalette palette;
  palette.setBrush(QPalette::Base,
                   pixmap.scaled(size(), Qt::IgnoreAspectRatio));
  graphics_view->setPalette(palette);
}

void View::ChangeFrame() {
  static constexpr uint8_t number_of_frames = 16;
  current_frame_++;
  current_frame_ %= number_of_frames;
  SetBackgroundImage(game_graphics_, FileLoader::GetFile<QPixmap>(
      ":/club_level/background/background_frame_"
          + QString::number(current_frame_) + ".png"));
  // TODO(Adamenko-Vladislav) frames don't change
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::Instance().keyPressEvent(event);
}

View& View::Instance() {
  static View instance;
  return instance;
}

void View::ShowGame() {
  game_graphics_ = new QGraphicsView(game_scene_);
  SetUpGraphicsView(game_graphics_);
  SetBackgroundImage(game_graphics_, FileLoader::GetFile<QPixmap>(
      ":/club_level/background/background_frame_0.png"));
  setCentralWidget(game_graphics_);
  qApp->setStyleSheet(FileLoader::CastFileToString(":/main.qss"));
  // TODO(Adamenko-Vladislav) set correct style
}

void View::ShowMainMenu() {
  menu_graphics_ = new QGraphicsView(menu_scene_);
  SetUpGraphicsView(menu_graphics_);
  SetBackgroundImage(menu_graphics_, FileLoader::GetFile<QPixmap>(
      ":/menu/main_menu_background.jpg"));
  // TODO(Adamenko-Vladislav) set correct image
  setCentralWidget(menu_graphics_);
  qApp->setStyleSheet(FileLoader::CastFileToString(":/menu/start_menu.qss"));
  // TODO(Adamenko-Vladislav) set correct style
}

void View::ShowSettings() {
  settings_graphics_ = new QGraphicsView(settings_scene_);
  SetUpGraphicsView(settings_graphics_);
  setCentralWidget(settings_graphics_);
  qApp->setStyleSheet(FileLoader::CastFileToString(":/settings/settings.qss"));
  // TODO(Adamenko-Vladislav) set correct style
}

void View::CustomGameScene() {
  game_scene_->addLine(0, 0, width(), height(), QPen(Qt::transparent));

  proxy_permit_->setGeometry(QRectF(
      kMargin,
      kMargin,
      permit_button_->width(),
      permit_button_->height()));

  proxy_reject_->setGeometry(QRectF(
      kMargin,
      permit_button_->geometry().bottom() + kMargin,
      reject_button_->width(),
      reject_button_->height()));

  proxy_errors_->setGeometry(QRectF(
      kMargin,
      reject_button_->geometry().bottom() + kMargin,
      errors_->width() + 100,
      errors_->height() + 100));
  // TODO(Adamenko-Vladislav) Magic constant.

  SetErrorsCount(0);
  SetTimer();
}

void View::CustomMainMenu() {
  settings_scene_->addLine(0, 0, width(), height(), QPen(Qt::transparent));

  proxy_stat_game_->setGeometry(QRectF(
      width() / 2 - width() / 1.5,
      height() / 2 - height() / 1.5,
      width() / 3,
      height() / 8));

  proxy_open_settings_->setGeometry(QRectF(
      start_game_->geometry().bottomLeft().x(),
      start_game_->geometry().bottomLeft().y() + kMargin,
      start_game_->width(),
      start_game_->height()));
}

void View::CustomSettings() {
  settings_scene_->addLine(0, 0, width(), height(), QPen(Qt::transparent));

  proxy_exit_settings_->setGeometry(QRect(
      width() - width() / 4 - kMargin,
      height() - height() / 10 - kMargin,
      width() / 4,
      height() / 10));

  proxy_complexity_->setGeometry(QRect(
      width() / 2 - width() / 8,
      height() / 2 - height() / 4,
      width() / 4,
      height() / 10));

  proxy_exit_shortcut_->setGeometry(QRectF(
      proxy_complexity_->scenePos().x(),
      proxy_complexity_->scenePos().y() + height() / 10 + kMargin,
      width() / 4,
      height() / 10));

  proxy_sound_->setGeometry(QRectF(
      proxy_exit_shortcut_->scenePos().x(),
      proxy_exit_shortcut_->scenePos().y() + height() / 10 + kMargin,
      width() / 4,
      height() / 10));

  proxy_default_settings_->setGeometry(QRectF(
      proxy_sound_->scenePos().x(),
      proxy_sound_->scenePos().y() + height() / 10 + kMargin,
      width() / 4,
      height() / 10));
}

void View::SetUpGraphicsView(QGraphicsView* graphics) {
  graphics->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphics->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
