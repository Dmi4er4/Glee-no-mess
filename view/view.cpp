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
  show();
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

void View::RequestKeyComboEnter() {
  shortcut_request_overlay_ = new QWidget;
  auto proxy = settings_scene_->addWidget(shortcut_request_overlay_);
  proxy->setGeometry(QRect{0, 0, width(), height()});
  auto layout = new QVBoxLayout;
  shortcut_request_overlay_->setLayout(layout);
  auto label = new QLabel("Please enter new hotkey:");
  label->setAlignment(Qt::AlignCenter);
  shortcut_request_overlay_->setStyleSheet("background: rgba(0, 0, 0, 100)");
  label->setStyleSheet("background: transparent");
  layout->addWidget(label);
  view_->setEnabled(false);
}

void View::HideShortcutRequestOverlay() {
  shortcut_request_overlay_->hide();
  view_->setEnabled(true);
}

void View::ShowGame() {
  view_->setScene(game_scene_);
}

void View::ShowMainMenu() {
  view_->setScene(menu_scene_);
}

void View::ShowSettings() {
  view_->setScene(settings_scene_);
}

void View::InitView() {
  QFontDatabase::addApplicationFont(":Koulen-Regular.ttf");
  auto screen_size = QGuiApplication::primaryScreen()->size();
  setFixedSize(screen_size);
  auto font_size = QString::number(
      static_cast<int>(screen_size.height() * .05));
  qApp->setStyleSheet("* { font-size: " + font_size + "px; }" +
                      FileLoader::CastFileToString(":/style.qss"));
  setWindowState(windowState() | Qt::WindowFullScreen);
  view_ = new QGraphicsView;
  DisableScrollbars(view_);
  setCentralWidget(view_);
}

void View::InitGameScene() {
  game_scene_ = new QGraphicsScene;

  LoadBackgroundFrames(":/club_level/background");
  game_background_ = game_scene_->addPixmap(background_frames_.first());

  auto proxy = game_scene_->addWidget(new QWidget);
  auto layout = new QHBoxLayout;
  proxy->widget()->setLayout(layout);

  layout->addWidget(permit_button_ = new QPushButton("OK"));
  layout->addWidget(reject_button_ = new QPushButton("NOT OK"));
  layout->addWidget(errors_ = new QLabel("Placeholder"));
  layout->addItem(new QSpacerItem(0, 0,
                                  QSizePolicy::Expanding,
                                  QSizePolicy::Expanding));

  permit_button_->setObjectName("in-game");
  reject_button_->setObjectName("in-game");

  proxy->setGeometry(QRectF{
    0,
    height() * .9,
    width() * 1.,
    height() * .1
  });

  SetErrorsCount(0);
  SetTimer();
}

void View::InitMainMenu() {
  menu_scene_ = new QGraphicsScene;
  menu_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
      ":menu/main_menu_background.jpg")
          .scaled(width(), height(), Qt::IgnoreAspectRatio));
  auto* proxy = menu_scene_->addWidget(new QWidget);
  auto layout = new QVBoxLayout;
  proxy->widget()->setLayout(layout);
  layout->addWidget(start_game_ = new QPushButton("Play"));
  layout->addWidget(open_settings_ = new QPushButton("Settings"));
  layout->addWidget(quit_ = new QPushButton("Quit"));

  start_game_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  proxy->setGeometry(QRectF{
      width() * .3,
      height() * .3,
      width() * .4,
      height() * .4
  });
}

void View::InitSettings() {
  settings_scene_ = new QGraphicsScene;
  settings_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
      ":menu/main_menu_background.jpg")
          .scaled(width(), height(), Qt::IgnoreAspectRatio));

  auto proxy = settings_scene_->addWidget(new QWidget);
  proxy->setGeometry(QRectF{
      width() * .25,
      height() * .2,
      width() * .5,
      height() * .6
  });
  auto form = new QFormLayout;
  proxy->widget()->setLayout(form);

  form->addRow(QLabelOrientate("Difficulty:", Qt::AlignRight),
               difficulty_ = new QPushButton);
  form->addRow(QLabelOrientate("Sound:", Qt::AlignRight),
               sound_ = new QPushButton);
  form->addRow(QLabelOrientate("Keybindings", Qt::AlignCenter));
  form->addRow(QLabelOrientate("Quit:", Qt::AlignRight),
               exit_shortcut_ = new QPushButton);
  form->addRow(reset_defaults_ = new QPushButton("Reset Defaults"));
  form->addRow(back_to_menu_ = new QPushButton("Back to menu"));
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

QLabel* View::QLabelOrientate(const QString& text, Qt::Alignment align) {
  auto* res = new QLabel(text);
  res->setAlignment(align);
  return res;
}
