#include "view.h"

#include "controller.h"
#include "file_loader.h"
#include "settings.h"

View::View() {
  InitView();
  InitGameScene();
  InitMainMenu();
  InitChooseGame();
  InitSettings();
  InitCasino();
  InitBlackJack();
  InitFruitMachine();
  InitShop();
  // ShowMainMenu();
  ShowShop();
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
  static const QString kInGameObjects = "in-game";
  game_scene_ = new QGraphicsScene;

  LoadBackgroundFrames(":/levels/club_level/background");
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
  layout->addWidget(to_menu_from_game_button_ =
                        new QPushButton("Back to menu"));
  layout->addWidget(guests_left_ = new QLabel("Visitors: "));
  layout->addWidget(day_ = new QLabel("Day: "));
  layout->addWidget(time_left_ = new QLabel("Time: "));

  guests_left_->setObjectName(kInGameObjects);
  day_->setObjectName(kInGameObjects);
  time_left_->setObjectName(kInGameObjects);

  guests_left_->setFixedSize(width() * 0.13, height() * 0.1);
  time_left_->setFixedSize(width() * 0.115, height() * 0.1);

  permit_button_->setObjectName(kInGameObjects);
  reject_button_->setObjectName(kInGameObjects);
  to_menu_from_game_button_->setObjectName(kInGameObjects);

  proxy->setGeometry(QRectF{
    0,
    height() * 0.9,
    width() * 1.0,
    height() * 0.1
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
      width() * 0.3,
      height() * 0.3,
      width() * 0.4,
      height() * 0.4
  });
}

void View::InitChooseGame() {
  choose_game_scene_ = new QGraphicsScene;
  choose_game_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
                             ":menu/main_menu_background.jpg")
                             .scaled(width(), height(), Qt::IgnoreAspectRatio));
  auto* proxy = choose_game_scene_->addWidget(new QWidget);
  auto layout = new QVBoxLayout;
  proxy->widget()->setLayout(layout);
  layout->addWidget(continue_button_ = new QPushButton("Continue"));
  layout->addWidget(new_game_button_ = new QPushButton("New game"));
  layout->addWidget(to_menu_from_choose_game_button_ =
                        new QPushButton("Back to menu"));

  continue_button_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  proxy->setGeometry(QRectF{
      width() * 0.3,
      height() * 0.3,
      width() * 0.4,
      height() * 0.4
  });
}

void View::InitSettings() {
  settings_scene_ = new QGraphicsScene;
  settings_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
      ":menu/main_menu_background.jpg")
          .scaled(width(), height(), Qt::IgnoreAspectRatio));

  auto proxy = settings_scene_->addWidget(new QWidget);
  proxy->setGeometry(QRectF{
      width() * 0.25,
      height() * 0.2,
      width() * 0.5,
      height() * 0.6
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

void View::InitCasino() {
  casino_scene_ = new QGraphicsScene;
  casino_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
      ":casino/casino.jpg").scaled(width(), height(), Qt::IgnoreAspectRatio));

  auto proxy = casino_scene_->addWidget(new QWidget);
  proxy->setGeometry(QRectF{
      width() * 0.25,
      height() * 0.3,
      width() * 0.5,
      height() * 0.6
  });
  auto form = new QFormLayout;
  proxy->widget()->setLayout(form);

  form->addRow(black_jack_ = new QPushButton("Black Jack"));
  form->addRow(fruit_machine_ = new QPushButton("Fruit machine"));
  form->addRow(casino_exit_ = new QPushButton("Exit"));
}

void View::InitBlackJack() {
  black_jack_scene_ = new QGraphicsScene;
  black_jack_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
      ":casino/black_jack.jpg").
      scaled(width(), height(), Qt::IgnoreAspectRatio));


  auto proxy_status = black_jack_scene_->addWidget(status_ = new QLabel);
  proxy_status->setGeometry(QRectF{
    0,
    0,
    width() * 0.3,
    height() * 0.1
  });

  auto croupier_proxy = black_jack_scene_->
      addWidget(croupier_ = new QLabel);
  QPixmap croupier = FileLoader::GetFile<QPixmap>(":casino/croupier.png");
  croupier_->setPixmap(croupier);


  croupier_proxy->setGeometry(QRectF{
      width() * 0.5 - croupier.width() * 0.5,
      0,
      width() * 0.2,
      height() * 0.3
  });

  constexpr int kCardsCount = 11;
  croupier_cards_.resize(kCardsCount);
  for (int i = 0; i < kCardsCount; ++i) {
    auto proxy_card = black_jack_scene_->addWidget(
        croupier_cards_[i] = new QLabel);

    proxy_card->setGeometry(QRectF{
        i * 80.0 + kMargin,
        croupier_proxy->geometry().height() + 15 * kMargin,
        width() * 0.35,
        height() * 0.35
    });
  }

  player_cards_.resize(kCardsCount);
  for (int i = 0; i < kCardsCount; ++i) {
    auto proxy_card = black_jack_scene_->addWidget(
        player_cards_[i] = new QLabel);

    proxy_card->setGeometry(QRectF{
        width() * 0.5 + i * 80.0 + kMargin,
        croupier_proxy->geometry().height() + 15 * kMargin,
        width() * 0.35,
        height() * 0.35
    });
  }

  auto proxy = black_jack_scene_->addWidget(new QWidget);
  proxy->setGeometry(QRectF{
      0,
      height() * 0.7,
      width() * 1.0,
      height() * 0.3
  });
  auto form = new QGridLayout;
  proxy->widget()->setLayout(form);

  form->addWidget(make_bid_ = new QPushButton("Make bid!"), 1, 0);
  form->addWidget(player_money_ = new QLabel, 0, 0);

  form->addWidget(bid_ = new QSpinBox, 1, 1);
  bid_->setMaximumWidth(width() * 0.125);
  bid_->setMinimum(1);
  bid_->setMaximum(INT32_MAX);

  form->addWidget(hit_me_ = new QPushButton("Hit me!"), 1, 2);
  hit_me_->hide();
  form->addWidget(stand_ = new QPushButton("Stand!"), 1, 3);
  stand_->hide();
  form->addWidget(back_to_casino_ = new QPushButton("Exit"), 1, 4);
}

void View::InitFruitMachine() {
  fruit_machine_scene_ = new QGraphicsScene;
  fruit_machine_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
      ":casino/fruit_machine.jpg").
      scaled(width(), height(), Qt::IgnoreAspectRatio));

  constexpr QSize size = {200, 150};

  for (int i : {0, 1, 2}) {
    auto& slot = slots_[i];
    auto proxy = fruit_machine_scene_->addWidget(slot = new QLabel);
    proxy->setGeometry({
        width() * .1 + i * (size.width() + kMargin),
        height() * .1,
        size.width(),
        size.height()
    });
    slot->setPixmap(FileLoader::GetFile<QPixmap>(":casino/machine_1.png"));
  }

  auto proxy = fruit_machine_scene_->addWidget(new QWidget);
  proxy->setGeometry(QRectF{
    width() * 0.1,
    height() * 0.3,
    width() * 0.7,
    height() * 0.4
  });

  auto form = new QGridLayout;
  proxy->widget()->setLayout(form);

  form->addWidget(bid_fruit_machine_ = new QSpinBox, 1, 0);
  bid_fruit_machine_->setMaximumWidth(width() * 0.125);
  bid_fruit_machine_->setMinimum(1);
  bid_fruit_machine_->setMaximum(INT32_MAX);

  form->addWidget(make_bid_fruit_machine_ =
      new QPushButton("Make bid!"), 1, 1, 1, -1);
  form->addWidget(money_ = new QLabel, 2, 0, 1, -1);
  form->addWidget(exit_fruit_machine_ = new QPushButton("Exit"), 3, 0, 1, -1);
}

void View::InitShop() {
  shop_scene_ = new QGraphicsScene;
  shop_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
      ":shop/shelves.png").
      scaled(width(), height(), Qt::IgnoreAspectRatio));

  auto proxy_shop_money = shop_scene_->addWidget(
      shop_money_ = new QLabel);
  proxy_shop_money->setGeometry({
    kMargin,
    kMargin,
    width() * 0.3,
    height() * 0.1
  });

  {
    auto proxy = shop_scene_->addWidget(new QWidget);
    proxy->setGeometry(QRectF{
        width() * 0.8,
        height() * 0.85,
        width() * 0.2,
        height() * 0.15
    });
    auto form = new QHBoxLayout;
    proxy->widget()->setLayout(form);
    form->addWidget(exit_shop_ = new QPushButton("Exit"));
  }

  static const size_t kWidth = static_cast<size_t>(width() * 0.3);
  static const size_t kHeight = static_cast<size_t>(height() * 0.3);
  static const size_t kItemWidth = kWidth / 4;
  static const size_t kItemHeight = kHeight / 2;

  static const QString kCost = "∰300";

  shelves_.resize(kShelves);
  for (int i = 0; i < kShelves; ++i) {
    shelves_[i].resize(kShelves);
    for (int j = 0; j < kShelves; ++j) {
      auto proxy = shop_scene_->addWidget(new QWidget);
      proxy->setGeometry(QRectF{
          width() * 0.1 + i * kWidth,
          height() * 0.1 + j * kHeight + kItemHeight + kMargin,
          kWidth / 2.0,
          kHeight / 4.0
      });
      auto form = new QHBoxLayout;
      proxy->widget()->setLayout(form);
      form->addWidget(shelves_[i][j] = new QPushButton(kCost));
      shelves_[i][j]->hide();
    }
  }

  items_.resize(kShelves);
  for (int i = 0; i < kShelves; ++i) {
    items_[i].resize(kShelves);
    for (int j = 0; j < kShelves; ++j) {
      auto proxy = shop_scene_->addWidget(new QWidget);
      proxy->setGeometry(QRectF{
          width() * 0.1 + i * kWidth + kItemWidth / 2,
          height() * 0.1 + j * kHeight,
          kItemWidth,
          kItemHeight
      });
      auto form = new QHBoxLayout;
      proxy->widget()->setLayout(form);
      form->addWidget(items_[i][j] = new QLabel);
    }
  }

  const QString kInShop = "in-shop";
  exit_shop_->setObjectName(kInShop);

  items_[kStandRow][kStandCol]->setPixmap(FileLoader::GetFile<QPixmap>(
      ":shop/stand_the_world.png").scaled(kItemWidth,
                                      kItemHeight,
                                         Qt::IgnoreAspectRatio));
  shelves_[kStandRow][kStandCol]->setObjectName(kInShop);
  shelves_[kStandRow][kStandCol]->show();

  items_[kKsivaRow][kKsivaCol]->setPixmap(FileLoader::GetFile<QPixmap>(
      ":shop/ksiva.png").scaled(kItemWidth,
                                      kItemHeight,
                                      Qt::IgnoreAspectRatio));
  shelves_[kKsivaRow][kKsivaCol]->setObjectName(kInShop);
  shelves_[kKsivaRow][kKsivaCol]->show();

  items_[kPandemicRow][kPandemicCol]->setPixmap(FileLoader::GetFile<QPixmap>(
      ":shop/pandemic.png").scaled(kItemWidth,
                                      kItemHeight,
                                      Qt::IgnoreAspectRatio));
  shelves_[kPandemicRow][kPandemicCol]->setObjectName(kInShop);
  shelves_[kPandemicRow][kPandemicCol]->show();

  items_[kVabankRow][kVabankCol]->setPixmap(FileLoader::GetFile<QPixmap>(
      ":shop/vabank.png").scaled(kItemWidth,
                                kItemHeight,
                                Qt::IgnoreAspectRatio));
  shelves_[kVabankRow][kVabankCol]->setObjectName(kInShop);
  shelves_[kVabankRow][kVabankCol]->show();
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
