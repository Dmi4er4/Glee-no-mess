#include "view.h"

#include "controller.h"
#include "file_loader.h"
#include "settings.h"

View::View() {
  InitView();
  InitGameScene();
  InitMainMenu();
  InitSettings();
  ShowMainMenu();
}

void View::SetTimer() {
  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &View::ChangeFrame);
  timer->start(1000 / kFrameRate);
}

void View::ChangeFrame() {
  if (++bg_frame_index_ == background_frames_.size()) {
    bg_frame_index_ = 0;
  }
  game_background_->setPixmap(background_frames_[bg_frame_index_]);
}

View& View::Instance() {
  static View instance;
  return instance;
}

void View::ShowGame() {
  view_->setScene(game_scene_);
  qApp->setStyleSheet(FileLoader::CastFileToString(":/main.qss"));
}

void View::ShowMainMenu() {
  view_->setScene(menu_scene_);
  qApp->setStyleSheet(FileLoader::CastFileToString(":/menu/start_menu.qss"));
}

void View::ShowSettings() {
  view_->setScene(settings_scene_);
  qApp->setStyleSheet(FileLoader::CastFileToString(":/settings/settings.qss"));
}

void View::InitView() {
  auto screen_size = QGuiApplication::primaryScreen()->size();
  setFixedSize(screen_size);
  setWindowState(windowState() | Qt::WindowFullScreen);
  view_ = new QGraphicsView;
  view_->setStyleSheet("border: none");
  DisableScrollbars(view_);
  setCentralWidget(view_);
}

void View::InitGameScene() {
  game_scene_ = new QGraphicsScene;

  LoadBackgroundFrames(":/club_level/background");
  game_background_ = game_scene_->addPixmap(background_frames_.first());

  proxy_permit_ = game_scene_->addWidget(
      permit_button_ = new QPushButton("OK"));
  proxy_reject_ = game_scene_->addWidget(
      reject_button_ = new QPushButton("NOT OK"));
  proxy_errors_ = game_scene_->addWidget(
      errors_ = new QLabel("Placeholder"));

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
      width(),
      errors_->height()));

  SetErrorsCount(0);
  SetTimer();
}

void View::InitMainMenu() {
  menu_scene_ = new QGraphicsScene;
  proxy_stat_game_ = menu_scene_->addWidget(
      start_game_ = new QPushButton("Play"));
  proxy_open_settings_ = menu_scene_->addWidget(
      open_settings_ = new QPushButton("Settings"));
  menu_scene_->addLine(0, 0, width(), height(), QPen(Qt::transparent));

  proxy_stat_game_->setGeometry(QRectF(
      width() * .2,
      height() * .2,
      width() * .6,
      height() * .3));

  proxy_open_settings_->setGeometry(QRectF(
      start_game_->geometry().bottomLeft().x(),
      start_game_->geometry().bottomLeft().y() + kMargin,
      start_game_->width(),
      start_game_->height()));
}

void View::InitSettings() {
  settings_scene_ = new QGraphicsScene;
  exit_settings_ = new QPushButton("Main menu");
  difficulty_ = new QPushButton;
  exit_shortcut_ = new QLabel(kExitShortcutText);
  sound_ = new QPushButton;
  default_settings_ = new QPushButton("Set default settings");
  proxy_exit_settings_ = settings_scene_->addWidget(exit_settings_);
  proxy_difficulty_ = settings_scene_->addWidget(difficulty_);
  proxy_exit_shortcut_ = settings_scene_->addWidget(exit_shortcut_);
  proxy_sound_ = settings_scene_->addWidget(sound_);
  proxy_default_settings_ = settings_scene_->addWidget(default_settings_);
  settings_scene_->addLine(0, 0, width(), height(), QPen(Qt::transparent));

  proxy_exit_settings_->setGeometry(QRect(
      width() - width() *  .25 - kMargin,
      height() - height() * .1 - kMargin,
      width() *  .25,
      height() * .1));

  proxy_difficulty_->setGeometry(QRect(
      width() * .5 - width() *  .125,
      height() * .5 - height() *  .25,
      width() *  .25,
      height() * .1));

  proxy_exit_shortcut_->setGeometry(QRectF(
      proxy_difficulty_->scenePos().x(),
      proxy_difficulty_->scenePos().y() + height() * .1 + kMargin,
      width() *  .25,
      height() * .1));

  proxy_sound_->setGeometry(QRectF(
      proxy_exit_shortcut_->scenePos().x(),
      proxy_exit_shortcut_->scenePos().y() + height() * .1 + kMargin,
      width() *  .25,
      height() * .1));

  proxy_default_settings_->setGeometry(QRectF(
      proxy_sound_->scenePos().x(),
      proxy_sound_->scenePos().y() + height() * .1 + kMargin,
      width() *  .25,
      height() * .1));
}

void View::DisableScrollbars(QGraphicsView* graphics) {
  graphics->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphics->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::Instance().keyPressEvent(event);
}

void View::LoadBackgroundFrames(const QString& folder) {
  bg_frame_index_ = 0;
  QDirIterator it(folder);
  QList<QString> frames;
  while (it.hasNext()) {
    frames.push_back(it.next());
  }
  frames.sort();
  background_frames_.clear();
  for (const auto& filename : frames) {
    background_frames_.emplace_back(
        FileLoader::GetFile<QPixmap>(filename)
            .scaled(width(), height(), Qt::IgnoreAspectRatio));
  }
}
