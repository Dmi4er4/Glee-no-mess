#include "view.h"

#include "controller.h"
#include "file_loader.h"
#include "settings.h"
#include "polyline_animation.h"
#include "model.h"

View::View() {
  SetTimer();
  InitView();
  InitGameScene();
  InitMainMenu();
  InitChooseGame();
  InitSettings();
  InitCasino();
  InitBlackJack();
  InitFruitMachine();
  InitShop();
  ManageSounds();
  ShowMainMenu();
  this->show();
}

void View::SetTimer() {
  frame_timer_ = new QTimer(this);
  connect(frame_timer_, &QTimer::timeout, this, &View::ChangeFrame);
  frame_timer_->start(kFrameDelay);
}

void View::ChangeFrame() {
  if (background_frames_.empty()) {
    return;
  }
  Model::Instance().DoAnimation(kFrameDelay);
  if (++bg_frame_index_ == background_frames_.size()) {
    bg_frame_index_ = 0;
  }
  game_background_->setPixmap(background_frames_[bg_frame_index_]);
  UpdateLevelStats();
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

void View::SetFruitMachineSlot(int index, const QPixmap& picture) {
  auto slot = slots_[index];
  slot->setPixmap(picture.scaled(
      slot->width(), slot->height(), Qt::IgnoreAspectRatio));
}

void View::SetFruitMachineSlotBorder(int index, bool is_spinning) {
  auto slot = slots_borders_[index];
  static auto stopped_slot_border = FileLoader::GetFile<QPixmap>(
      ":casino/stopped_slot_border.png");
  static auto spinning_slot_border = FileLoader::GetFile<QPixmap>(
      ":casino/spinning_slot_border.png");
  if (is_spinning) {
    slot->setPixmap(spinning_slot_border.scaled(
        slot->width(), slot->height(), Qt::IgnoreAspectRatio));
  } else {
    slot->setPixmap(stopped_slot_border.scaled(
        slot->width(), slot->height(), Qt::IgnoreAspectRatio));
  }
}

void View::InitView() {
  QFontDatabase::addApplicationFont(":Koulen-Regular.ttf");
  auto screen_size = QGuiApplication::primaryScreen()->size();
  setFixedSize(screen_size);
  auto font_size = QString::number(
      static_cast<int>(screen_size.height() * .05));
  auto tooltip_font_size = QString::number(
      static_cast<int>(screen_size.height() * .03));
  qApp->setStyleSheet("* { font-size: " + font_size + "px; }" +
                      "QToolTip { font-size: " + tooltip_font_size + "px; }" +
                      "QLabel#in-intro { font-size: " +
                      tooltip_font_size + "px; }" +
                      FileLoader::CastFileToString(":/style.qss"));
  setWindowState(windowState() | Qt::WindowFullScreen);
  view_ = new QGraphicsView;
  view_->setSceneRect(geometry());
  DisableScrollbars(view_);
  setCentralWidget(view_);
}

void View::InitGameScene() {
  {
    static const QString kInGameObjects = "in-game";
    game_scene_ = new QGraphicsScene;

    game_background_ = game_scene_->addPixmap(QPixmap());

    dudes_father_ = game_scene_->createItemGroup({});

    auto proxy = game_scene_->addWidget(new QWidget);
    auto layout = new QHBoxLayout;
    proxy->widget()->setLayout(layout);

    layout->addWidget(permit_button_ = new QPushButton("OK"));
    layout->addWidget(reject_button_ = new QPushButton("NOT OK"));
    layout->addWidget(lives_ = new QLabel("Placeholder"));
    layout->addItem(new QSpacerItem(0, 0,
                                    QSizePolicy::Expanding,
                                    QSizePolicy::Expanding));
    layout->addWidget(game_pause_button_ =
                          new QPushButton("Pause"));
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
    game_pause_button_->setObjectName(kInGameObjects);

    proxy->setGeometry(QRectF{
        0,
        height() * 0.893,
        width() * 1.0,
        height() * 0.1
    });
  }

  {
    game_pause_overlay = new QWidget;
    auto proxy = game_scene_->addWidget(game_pause_overlay);
    proxy->setGeometry(QRect{0, 0, width(), height()});
    auto layout = new QVBoxLayout;

    game_pause_overlay->setLayout(layout);
    game_exit_ = new QPushButton("   Back to main menu   ");
    game_continue_ = new QPushButton("Continue");

    layout->setAlignment(Qt::AlignmentFlag::AlignCenter);

    layout->addWidget(game_exit_);
    layout->addWidget(game_continue_);
    game_pause_overlay->setStyleSheet("background: rgba(0, 0, 0, 100)");
    game_pause_overlay->hide();
  }

  {
    game_over_overlay_ = new QWidget;
    auto proxy = game_scene_->addWidget(game_over_overlay_);
    proxy->setGeometry(QRect{0, 0, width(), height()});
    auto layout = new QVBoxLayout;
    game_over_overlay_->setLayout(layout);

    to_main_menu_ = new QPushButton("   Back to main menu   ");
    to_casino_ = new QPushButton("Casino");
    to_shop_ = new QPushButton("Shop");
    status_label_ = new QLabel;
    status_label_->setAlignment(Qt::AlignCenter);
    layout->setAlignment(Qt::AlignmentFlag::AlignCenter);

    layout->addWidget(status_label_);
    layout->addWidget(to_main_menu_);
    layout->addWidget(to_casino_);
    layout->addWidget(to_shop_);
    game_over_overlay_->setStyleSheet("background: rgba(0, 0, 0, 100)");
    game_over_overlay_->hide();
  }

  {
    game_intro_overlay_ = new QWidget;
    auto proxy = game_scene_->addWidget(game_intro_overlay_);
    proxy->setGeometry(QRect{0, 0, width(), height()});
    auto layout = new QVBoxLayout;
    game_intro_overlay_->setLayout(layout);

    intro_ = new QLabel;
    static const QString name = "in-intro";
    intro_->setObjectName(name);
    intro_->setWordWrap(true);
    layout->setAlignment(Qt::AlignmentFlag::AlignCenter);

    layout->addWidget(intro_);
    game_intro_overlay_->setStyleSheet("background: rgba(0, 0, 0, 100)");
    game_intro_overlay_->hide();
  }

  {
    glee_no_mess_ = new QGraphicsPixmapItem(
        FileLoader::GetFile<QPixmap>(":/levels/glee_no_mess.png").scaled(
        width() * 0.15, height() * 0.3, Qt::IgnoreAspectRatio));
    dudes_father_->addToGroup(glee_no_mess_);
    glee_no_mess_->setPos(width() * 0.25, height() * 0.6);
  }
}

void View::InitMainMenu() {
  menu_scene_ = new QGraphicsScene;

  {
    QLabel* gif_anim = new QLabel();
    QMovie* movie = new QMovie(":menu/main_menu.gif");
    movie->setScaledSize(size());
    gif_anim->setMovie(movie);
    gif_anim->setGeometry({0, 0, width(), height()});
    movie->start();
    menu_scene_->addWidget(gif_anim);
  }

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

  {
    QLabel* gif_anim = new QLabel();
    QMovie* movie = new QMovie(":menu/main_menu.gif");
    movie->setScaledSize(size());
    gif_anim->setMovie(movie);
    gif_anim->setGeometry({0, 0, width(), height()});
    movie->start();
    choose_game_scene_->addWidget(gif_anim);
  }

  auto* proxy = choose_game_scene_->addWidget(new QWidget);
  auto layout = new QVBoxLayout;
  proxy->widget()->setLayout(layout);
  layout->addWidget(continue_button_ = new QPushButton("Continue"));
  layout->addWidget(to_casino_from_choose_button_ = new QPushButton("Casino"));
  layout->addWidget(to_shop_from_choose_button_ = new QPushButton("Shop"));
  layout->addWidget(new_game_button_ = new QPushButton("New game"));
  layout->addWidget(to_menu_from_choose_game_button_ =
                        new QPushButton("Back"));

  continue_button_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  proxy->setGeometry(QRectF{
      width() * 0.3,
      height() * 0.2,
      width() * 0.4,
      height() * 0.6
  });
}

void View::InitSettings() {
  settings_scene_ = new QGraphicsScene;

  {
    QLabel* gif_anim = new QLabel();
    QMovie* movie = new QMovie(":settings/settings.gif");
    movie->setScaledSize(size());
    gif_anim->setMovie(movie);
    gif_anim->setGeometry({0, 0, width(), height()});
    movie->start();
    settings_scene_->addWidget(gif_anim);
  }

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
  casino_scene_->addPixmap(
      FileLoader::GetFile<QPixmap>(
          ":casino/best_casino.jpg").scaled(width(),
                  height(), Qt::IgnoreAspectRatio));

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
  form->addRow(casino_exit_ = new QPushButton("Back to menu"));
}

void View::InitBlackJack() {
  black_jack_scene_ = new QGraphicsScene;
  black_jack_scene_->addPixmap(FileLoader::GetFile<QPixmap>(
                                   ":casino/black_jack.jpg").
                               scaled(width(),
                                      height(), Qt::IgnoreAspectRatio));


  {
    auto proxy =
        black_jack_scene_->addWidget(new QWidget);
    proxy->setGeometry(QRectF{
        0,
        0,
        width() * 0.3,
        height() * 0.1
    });
    auto form = new QHBoxLayout;
    proxy->widget()->setLayout(form);
    form->addWidget(status_ = new QLabel);
  }

  auto croupier_proxy = black_jack_scene_->addWidget(new QWidget);;

  {
    auto croupier = FileLoader::GetFile<QPixmap>(":casino/croupier.png");
    croupier_proxy->setGeometry(QRectF{
        width() * 0.5 - croupier.width() * 0.5,
        0,
        width() * 0.2,
        height() * 0.3
    });
    auto form = new QHBoxLayout;
    croupier_proxy->widget()->setLayout(form);
    form->addWidget(croupier_ = new QLabel);
    croupier_->setPixmap(
        croupier.scaled(width() * 0.2, height() * 0.3, Qt::IgnoreAspectRatio));
  }

  constexpr int kCardsCount = 11;
  croupier_cards_.resize(kCardsCount);
  for (int i = 0; i < kCardsCount; ++i) {
    {
      auto proxy = black_jack_scene_->addWidget(new QWidget);
      proxy->setGeometry(QRectF{
          i * 80.0 + kMargin,
          croupier_proxy->geometry().height() + 15 * kMargin,
          width() * 0.35,
          height() * 0.35
      });
      auto form = new QHBoxLayout;
      proxy->widget()->setLayout(form);
      form->addWidget(croupier_cards_[i] = new QLabel);
    }
  }

  player_cards_.resize(kCardsCount);
  for (int i = 0; i < kCardsCount; ++i) {
    {
      auto proxy = black_jack_scene_->addWidget(new QWidget);
      proxy->setGeometry(QRectF{
          width() * 0.5 + i * 80.0 + kMargin,
          croupier_proxy->geometry().height() + 15 * kMargin,
          width() * 0.35,
          height() * 0.35
      });
      auto form = new QHBoxLayout;
      proxy->widget()->setLayout(form);
      form->addWidget(player_cards_[i] = new QLabel);
    }
  }

  {
    auto proxy = black_jack_scene_->addWidget(new QWidget);
    proxy->setGeometry(QRectF{
        0,
        height() * 0.8,
        width() * 1.0,
        height() * 0.2
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
}

void View::InitFruitMachine() {
  const QString kInFruit = "in-fruit";
  fruit_machine_scene_ = new QGraphicsScene;

  {
    QLabel* gif_anim = new QLabel();
    QMovie* movie = new QMovie(":casino/rich_background.gif");
    movie->setScaledSize(size());
    gif_anim->setMovie(movie);
    gif_anim->setGeometry({0, 0, width(), height()});
    movie->start();
    fruit_machine_scene_->addWidget(gif_anim);
  }

  const double rect_ratio = 2;
  QSize rect_size;
  rect_size.setWidth(std::min(width() * 0.8, height() * 0.5 * rect_ratio));
  rect_size.setHeight(rect_size.width() / rect_ratio);
  QSize size{static_cast<int>(rect_size.width() / 3.0), rect_size.height()};
  const double all_slots_left = width() * 0.5 - 0.5 * rect_size.width();
  const double all_slots_top = height() * 0.2;

  {
    auto& all_slots_border = all_slots_border_;
    auto proxy = fruit_machine_scene_->addWidget(all_slots_border = new QLabel);
    proxy->setGeometry({all_slots_left - rect_size.width() * 0.5,
                        all_slots_top - rect_size.height() * 1.0,
                        rect_size.width() * 2.0,
                        rect_size.height() * 3.0});
    all_slots_border->setPixmap(FileLoader::GetFile<QPixmap>
                                (":casino/all_slots_border.png").
                                scaled(all_slots_border->width(),
                                       all_slots_border->height(),
                                       Qt::IgnoreAspectRatio));
  }

  for (int i : {0, 1, 2}) {
    auto& slot_border = slots_borders_[i];
    auto proxy = fruit_machine_scene_->addWidget(slot_border = new QLabel);
    proxy->setGeometry({all_slots_left + i * (size.width()),
                        all_slots_top,
                        static_cast<qreal>(size.width()),
                        static_cast<qreal>(size.height())});
    slot_border->setPixmap(FileLoader::GetFile<QPixmap>
                           (":casino/stopped_slot_border.png").
                           scaled(slot_border->width(),
                                  slot_border->height(),
                                  Qt::IgnoreAspectRatio));
  }
  for (int i : {0, 1, 2}) {
    auto& slot = slots_[i];
    auto proxy = fruit_machine_scene_->addWidget(slot = new QLabel);
    proxy->setGeometry({
        all_slots_left + i * (size.width()) + size.width() * 0.25,
        all_slots_top + size.height() * 0.25,
        size.width() * 0.5,
        size.height() * 0.5
    });
    slot->setPixmap(FileLoader::GetFile<QPixmap>(":casino/machine_1.png").
                    scaled(slot->width(),
                           slot->height(), Qt::IgnoreAspectRatio));
  }

  auto proxy = fruit_machine_scene_->addWidget(new QWidget);
  proxy->setGeometry(QRectF{
      0.0,
      all_slots_top + rect_size.height() * 1.1,
      width() * 1.0,
      height() - (all_slots_top + rect_size.height() * 1.1)
  });

  auto form = new QGridLayout;
  proxy->widget()->setLayout(form);

  form->addWidget(bid_fruit_machine_ = new QSpinBox, 2, 2);
  bid_fruit_machine_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  bid_fruit_machine_->setMaximumWidth(width() * 0.125);
  bid_fruit_machine_->setMinimum(1);
  bid_fruit_machine_->setMaximum(INT32_MAX);

  form->addWidget(make_bid_fruit_machine_ =
                      new QPushButton("Make bid!"), 2, 3);
  form->addWidget(money_ = new QLabel, 3, 3);
  money_->setAlignment(Qt::AlignCenter);
  form->addWidget(exit_fruit_machine_ = new QPushButton("Exit"), 3, 0);

  bid_fruit_machine_->setMinimumWidth(width() * 0.5);
  money_->setMinimumWidth(width() * 0.3);

  make_bid_fruit_machine_->setObjectName(kInFruit);
  exit_fruit_machine_->setObjectName(kInFruit);
  money_->setObjectName(kInFruit);
  bid_fruit_machine_->setObjectName(kInFruit);

  form->setColumnStretch(0, 4);
  form->setColumnStretch(1, 3);
  form->setColumnStretch(2, 1);
  form->setColumnStretch(3, 0);

  form->setRowStretch(0, 1);
  form->setRowStretch(1, 3);
  form->setRowStretch(2, 1);
  form->setRowStretch(3, 0);
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
    form->addWidget(exit_shop_ = new QPushButton("Menu"));
  }

  static const size_t kWidth = static_cast<size_t>(width() * 0.3);
  static const size_t kHeight = static_cast<size_t>(height() * 0.3);
  static const size_t kItemWidth = kWidth / 4;
  static const size_t kItemHeight = kHeight / 2;

  static const QString kCost = "???300";

  shelves_.resize(kShelves);
  for (int i = 0; i < kShelves; ++i) {
    shelves_[i].resize(kShelves);
    for (int j = 0; j < kShelves; ++j) {
      auto proxy = shop_scene_->addWidget(new QWidget);
      proxy->setGeometry(QRectF{
          20.0 * kMargin + i * kWidth,
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
          20.0 * kMargin + i * kWidth + kItemWidth / 2,
          height() * 0.1 + j * kHeight,
          kItemWidth * 1.0,
          kItemHeight * 1.0
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

  items_[kStandRow][kStandCol]->setToolTip(
      "     Stand \"The World!\"  \nAdds 9 seconds to the end\nof the shift.");

  shelves_[kStandRow][kStandCol]->setObjectName(kInShop);
  shelves_[kStandRow][kStandCol]->show();

  items_[kKsivaRow][kKsivaCol]->
      setPixmap(FileLoader::GetFile<QPixmap>(
                    ":shop/ksiva.png").scaled(kItemWidth,
                            kItemHeight,
                            Qt::IgnoreAspectRatio));
  shelves_[kKsivaRow][kKsivaCol]->setObjectName(kInShop);
  shelves_[kKsivaRow][kKsivaCol]->show();

  items_[kKsivaRow][kKsivaCol]->setToolTip(
      "         Ksiva!  \nForgive first mistake.");

  items_[kPandemicRow][kPandemicCol]->setPixmap(FileLoader::GetFile<QPixmap>(
      ":shop/pandemic.png").scaled(kItemWidth,
                                      kItemHeight,
                                      Qt::IgnoreAspectRatio));
  shelves_[kPandemicRow][kPandemicCol]->setObjectName(kInShop);
  shelves_[kPandemicRow][kPandemicCol]->show();

  items_[kPandemicRow][kPandemicCol]->setToolTip(
      "         Pandemic!  \nReduces the number of\nguests by half.");

  items_[kVabankRow][kVabankCol]->setPixmap(FileLoader::GetFile<QPixmap>(
      ":shop/vabank.png").scaled(kItemWidth,
                                kItemHeight,
                                Qt::IgnoreAspectRatio));

  shelves_[kVabankRow][kVabankCol]->setObjectName(kInShop);
  shelves_[kVabankRow][kVabankCol]->show();

  items_[kVabankRow][kVabankCol]->setToolTip(
      "         All in!  \nif you win, salary x2\nif you loose, no salary.");
}

void View::ManageSounds() {
  {
    gachi_level_sound_ = new QMediaPlayer(this);
    auto audioOutput = new QAudioOutput(this);
    gachi_level_sound_->setAudioOutput(audioOutput);
    gachi_level_sound_->setSource(QUrl("qrc:/sound_effects/gachi_level.ogg"));
    audioOutput->setVolume(50);
    gachi_level_sound_->setLoops(QMediaPlayer::Infinite);
  }

  {
    bsu_level_sound_ = new QMediaPlayer(this);
    auto audioOutput = new QAudioOutput(this);
    bsu_level_sound_->setAudioOutput(audioOutput);
    bsu_level_sound_->setSource(QUrl("qrc:/sound_effects/bsu_level.ogg"));
    audioOutput->setVolume(50);
    bsu_level_sound_->setLoops(QMediaPlayer::Infinite);
  }

  {
    menu_sound_ = new QMediaPlayer(this);
    auto audioOutput = new QAudioOutput(this);
    menu_sound_->setAudioOutput(audioOutput);
    menu_sound_->setSource(QUrl("qrc:/sound_effects/menu.ogg"));
    audioOutput->setVolume(50);
    menu_sound_->setLoops(QMediaPlayer::Infinite);
  }

  {
    shop_sound_ = new QMediaPlayer(this);
    auto audioOutput = new QAudioOutput(this);
    shop_sound_->setAudioOutput(audioOutput);
    shop_sound_->setSource(QUrl("qrc:/sound_effects/shop.ogg"));
    audioOutput->setVolume(50);
    shop_sound_->setLoops(QMediaPlayer::Infinite);
  }

  {
    casino_sound_ = new QMediaPlayer(this);
    auto audioOutput = new QAudioOutput(this);
    casino_sound_->setAudioOutput(audioOutput);
    casino_sound_->setSource(QUrl("qrc:/sound_effects/casino.ogg"));
    audioOutput->setVolume(50);
    casino_sound_->setLoops(QMediaPlayer::Infinite);
  }

  {
    settings_sound_ = new QMediaPlayer(this);
    auto audioOutput = new QAudioOutput(this);
    settings_sound_->setAudioOutput(audioOutput);
    settings_sound_->setSource(QUrl("qrc:/sound_effects/settings.ogg"));
    audioOutput->setVolume(50);
    settings_sound_->setLoops(QMediaPlayer::Infinite);
  }
}

void View::DisableScrollbars(QGraphicsView* graphics) {
  graphics->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphics->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::Instance().keyPressEvent(event);
}

void View::LoadBackgroundFrames(const QString& folder) {
  frame_timer_->stop();
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
  frame_timer_->start(kFrameDelay);
}

QLabel* View::QLabelOrientate(const QString& text, Qt::Alignment align) {
  auto* res = new QLabel(text);
  res->setAlignment(align);
  return res;
}

void View::AddGuestSprite(Guest* guest, const QPixmap& pixmap) {
  auto item = new QGraphicsPixmapItem(
      pixmap.scaled(width() * 0.1, height() * 0.23, Qt::KeepAspectRatio)
          .transformed(QTransform().scale(-1, 1)));
  dudes_father_->addToGroup(item);
  guest->SetSprite(item);
}

void View::UpdateLevelStats() {
  auto level = Model::Instance().GetLevel();
  if (!level) {
    return;
  }
  auto lives = QString::number(level->GetLives());
  auto guests = QString::number(level->GetGuestsLeft());
  auto seconds = level->GetTimeLeft();
  auto minutes = seconds / 60;
  seconds %= 60;
  auto time = QString::number(minutes) + ":" +
              (seconds < 10 ? "0" : "") +
              QString::number(seconds);
  auto day = QString::number(level->GetDayIndex() + 1);
  lives_->setText("Lives: " + lives);
  guests_left_->setText("Visitors: " + guests);
  time_left_->setText(time);
  day_->setText("Day: " + day);
}

void View::StopAllSounds(QMediaPlayer* except) {
  if (except != gachi_level_sound_) {
    gachi_level_sound_->stop();
  }
  if (except != bsu_level_sound_) {
    bsu_level_sound_->stop();
  }
  if (except != casino_sound_) {
    casino_sound_->stop();
  }
  if (except != shop_sound_) {
    shop_sound_->stop();
  }
  if (except != settings_sound_) {
    settings_sound_->stop();
  }
  if (except != menu_sound_) {
    menu_sound_->stop();
  }
}

void View::ShowGame() {
  view_->setScene(game_scene_);
  game_exit_->setAttribute(Qt::WA_UnderMouse, false);
  game_continue_->setAttribute(Qt::WA_UnderMouse, false);
  permit_button_->setAttribute(Qt::WA_UnderMouse, false);
  reject_button_->setAttribute(Qt::WA_UnderMouse, false);

  if (sound_->text() == kOn) {
    StopAllSounds();
    gachi_level_sound_->play();
  }
}

void View::ShowMainMenu() {
  view_->setScene(menu_scene_);
  start_game_->setAttribute(Qt::WA_UnderMouse, false);
  open_settings_->setAttribute(Qt::WA_UnderMouse, false);
  quit_->setAttribute(Qt::WA_UnderMouse, false);

  if (sound_->text() == kOn) {
    StopAllSounds(menu_sound_);
    menu_sound_->play();
  }
}

void View::ShowSettings() {
  view_->setScene(settings_scene_);
  difficulty_->setAttribute(Qt::WA_UnderMouse, false);
  sound_->setAttribute(Qt::WA_UnderMouse, false);
  reset_defaults_->setAttribute(Qt::WA_UnderMouse, false);
  exit_shortcut_->setAttribute(Qt::WA_UnderMouse, false);
  back_to_menu_->setAttribute(Qt::WA_UnderMouse, false);

  if (sound_->text() == kOn) {
    StopAllSounds(settings_sound_);
    settings_sound_->play();
  }
}

void View::ShowCasino() {
  view_->setScene(casino_scene_);
  black_jack_->setAttribute(Qt::WA_UnderMouse, false);
  fruit_machine_->setAttribute(Qt::WA_UnderMouse, false);
  casino_exit_->setAttribute(Qt::WA_UnderMouse, false);

  if (sound_->text() == kOn) {
    StopAllSounds(casino_sound_);
    casino_sound_->play();
  }
}

void View::ShowBlackJack() {
  CloseBlackJackGame();
  for (auto to : croupier_cards_) {
    to->setPixmap(QPixmap());
  }
  for (auto to : player_cards_) {
    to->setPixmap(QPixmap());
  }
  status_->setText("");
  view_->setScene(black_jack_scene_);

  make_bid_->setAttribute(Qt::WA_UnderMouse, false);
  back_to_casino_->setAttribute(Qt::WA_UnderMouse, false);
  hit_me_->setAttribute(Qt::WA_UnderMouse, false);
  stand_->setAttribute(Qt::WA_UnderMouse, false);

  if (sound_->text() == kOn) {
    StopAllSounds(casino_sound_);
    casino_sound_->play();
  }
}

void View::ShowShop() {
  view_->setScene(shop_scene_);

  exit_shop_->setAttribute(Qt::WA_UnderMouse, false);
  shelves_[kStandRow][kStandCol]->setAttribute(Qt::WA_UnderMouse, false);
  shelves_[kKsivaRow][kKsivaCol]->setAttribute(Qt::WA_UnderMouse, false);
  shelves_[kPandemicRow][kPandemicCol]->
      setAttribute(Qt::WA_UnderMouse, false);
  shelves_[kVabankRow][kVabankCol]->setAttribute(Qt::WA_UnderMouse, false);

  if (sound_->text() == kOn) {
    StopAllSounds();
    shop_sound_->play();
  }
}

void View::ShowChooseGame() {
  view_->setScene(choose_game_scene_);
  continue_button_->setAttribute(Qt::WA_UnderMouse, false);
  new_game_button_->setAttribute(Qt::WA_UnderMouse, false);
  to_casino_from_choose_button_->setAttribute(Qt::WA_UnderMouse, false);
  to_shop_from_choose_button_->setAttribute(Qt::WA_UnderMouse, false);
  to_menu_from_choose_game_button_->setAttribute(Qt::WA_UnderMouse, false);

  continue_button_->setText("Continue");

  if (sound_->text() == kOn) {
    StopAllSounds(menu_sound_);
    menu_sound_->play();
  }
}

void View::ShowFruitMachine() {
  view_->setScene(fruit_machine_scene_);
  exit_fruit_machine_->setAttribute(Qt::WA_UnderMouse, false);
  make_bid_fruit_machine_->setAttribute(Qt::WA_UnderMouse, false);

  if (sound_->text() == kOn) {
    StopAllSounds(casino_sound_);
    casino_sound_->play();
  }
}
