#pragma once

#include <QAudioOutput>
#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>
#include <QFormLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMovie>
#include <QPushButton>
#include <QSoundEffect>
#include <QSpinBox>
#include <QStyle>
#include <QTimer>
#include <QTextEdit>

#include <algorithm>
#include <vector>

#include "guest.h"
#include "typedefs.h"
#include "settings.h"

class Model;
class Controller;

class View : public QMainWindow {
  Q_OBJECT

 public:
  static View& Instance();
  static constexpr int kMargin = 5;
  static constexpr size_t kShelves = 3;
  // reversed rows and columns!!!
  static constexpr int kStandRow = 0;
  static constexpr int kStandCol = 0;
  static constexpr int kKsivaRow = 1;
  static constexpr int kKsivaCol = 0;
  static constexpr int kPandemicRow = 1;
  static constexpr int kPandemicCol = 1;
  static constexpr int kVabankRow = 2;
  static constexpr int kVabankCol = 1;

  void keyPressEvent(QKeyEvent* event) override;

  // Game
  bool IsGame() const { return view_->scene() == game_scene_; }

  void AddGuestSprite(Guest* guest, const QPixmap& pixmap);

  void UpdateLevelStats();

  void SetTimer();
  void ChangeFrame();

  auto GetPermitButton() const { return permit_button_; }
  auto GetRejectButton() const { return reject_button_; }
  auto GetGameScene() const { return game_scene_; }
  auto GetPauseGameButton() const { return game_pause_button_; }

  void GamePauseStart() { game_pause_overlay->setHidden(false); }
  void GamePauseFinish() { game_pause_overlay->hide(); }

  void GameOverShow() { game_over_overlay_->setHidden(false); }
  void GameOverHide() { game_over_overlay_->hide(); }

  auto* GetToMainMenuButton() { return to_main_menu_; }
  auto* GetToCasinoButton() { return to_casino_; }
  auto* GetToShopButton() { return to_shop_; }

  void DisableGameButtons() {
    permit_button_->setEnabled(false);
    reject_button_->setEnabled(false);
    game_pause_button_->setEnabled(false);
  }

  void EnableGameButtons() {
    permit_button_->setEnabled(true);
    reject_button_->setEnabled(true);
    game_pause_button_->setEnabled(true);
  }

  void WinGame() { status_label_->setText("You won!"); }
  void LooseGame() { status_label_->setText("You loose!"); }

  void SetIntro(const QString& text) {
    intro_->setText(text + "\nPress enter to continue!");
  }

  void ShowIntro() { game_intro_overlay_->setHidden(false); }
  void HideIntro() {game_intro_overlay_->hide(); }

  auto* GetTimer() { return guests_; }

  // Menu
  bool IsMenu() const { return view_->scene() == menu_scene_; }
  auto GetOpenSettingsButton() const { return open_settings_; }
  auto GetStartGameButton() const { return start_game_; }

  // Choose game
  bool IsChooseGame() const { return view_->scene() == choose_game_scene_; }
  auto GetContinueButton() const { return continue_button_; }
  auto GetNewGameButton() const { return new_game_button_; }
  auto GetToMenuFromChooseGameButton() const {
    return to_menu_from_choose_game_button_;
  }
  auto GetToCasinoFromChooseButton() const {
    return to_casino_from_choose_button_;
  }
  auto GetToShopFromChooseButton() const {
    return to_shop_from_choose_button_;
  }

  // Settings
  bool IsSettings() const { return view_->scene() == settings_scene_; }
  void SetExitShortcut(const QString& text) { exit_shortcut_->setText(text); }
  void SetSound(const QString& text) { sound_->setText(text); }
  void SetDifficulty(const QString& text) { difficulty_->setText(text); }
  void RequestKeyComboEnter();
  void HideShortcutRequestOverlay();

  auto* GetBackToMenuButton() { return back_to_menu_; }
  auto* GetDifficultyButton() { return difficulty_; }
  auto* GetSoundButton() { return sound_; }
  auto* GetDefaultSettingsButton() { return reset_defaults_; }
  auto* GetExitShortcutButton() { return exit_shortcut_; }
  auto* GetQuitButton() { return quit_; }

  // BlackJack
  auto* GetExitCasinoButton() { return casino_exit_; }
  auto* GetBlackJackButton() {return black_jack_; }
  auto* GetFruitMachineButton() { return fruit_machine_; }
  auto* GetExitBlackJackButton() { return back_to_casino_; }
  auto* GetExitCasino() { return casino_exit_; }

  void SetPlayerMoney(money_t x) {
    QString text = "Your money: " + QString::number(x);
    player_money_->setText(text);
    money_->setText(text);
    shop_money_->setText(text);
  }

  void SetPlayerMaxBid(money_t x) {
    bid_->setMaximum(x);
    bid_fruit_machine_->setMaximum(x);
  }

  auto* GetMakeBidButton() { return make_bid_; }
  auto GetBid() { return bid_->value(); }
  auto* GetHitMeButton() { return hit_me_; }
  auto* GetStandButton() { return stand_; }
  auto* GetGameExitButton() { return game_exit_; }
  auto* GetGameContinueButton() { return game_continue_; }

  void ShowBlackJackGame() {
    hit_me_->show();
    stand_->show();
    for (auto to : croupier_cards_) {
      to->setPixmap(QPixmap());
    }
    for (auto to : player_cards_) {
      to->setPixmap(QPixmap());
    }
    status_->setText("");
    bid_->setValue(1);
  }

  void CloseBlackJackGame() {
    hit_me_->close();
    stand_->close();
  }

  void PutCroupierCard(size_t number, QPixmap card) {
    croupier_cards_[number]->setPixmap(
        card.scaled(width() * 0.11, height() * 0.3));
  }

  void PutPlayerCard(size_t number, QPixmap card) {
    player_cards_[number]->setPixmap(
        card.scaled(width() * 0.11, height() * 0.3));
  }

  void ShowSatus(const QString& status) {
    status_->setText(status);
    CloseBlackJackGame();
  }

  // Fruit Machine
  auto* GetExitFruitMachine() { return exit_fruit_machine_; }
  auto GetFruitMachineBid() { return bid_fruit_machine_->value(); }
  auto* GetMakeBidFruitMachine() { return make_bid_fruit_machine_; }

  void SetFruitMachineSlot(int index, const QPixmap& picture);
  void SetFruitMachineSlotBorder(int index, bool is_spinning);

  // Shop
  auto* GetStandTheWorldBuy() { return shelves_[kStandRow][kStandCol]; }
  auto* GetKsivaBuy() { return shelves_[kKsivaRow][kKsivaCol]; }
  auto* GetPandemicBuy() { return shelves_[kPandemicRow][kPandemicCol]; }
  auto* GetVabankBuy() { return shelves_[kVabankRow][kVabankCol]; }
  auto* GetExitShop() { return exit_shop_; }

  const QString kBought = "Bought";

  void HideStandTheWorld() {
    shelves_[kStandRow][kStandCol]->setText(kBought);
    shelves_[kStandRow][kStandCol]->setEnabled(false);
  }

  void HideKsive() {
    shelves_[kKsivaRow][kKsivaCol]->setText(kBought);
    shelves_[kKsivaRow][kKsivaCol]->setEnabled(false);
  }

  void HidePandemic() {
    shelves_[kPandemicRow][kPandemicCol]->setText(kBought);
    shelves_[kPandemicRow][kPandemicCol]->setEnabled(false);
  }

  void HideVabank() {
    shelves_[kVabankRow][kVabankCol]->setText(kBought);
    shelves_[kVabankRow][kVabankCol]->setEnabled(false);
  }

  // Show Scene
  void ShowGame();
  void ShowMainMenu();
  void ShowSettings();
  void ShowCasino();
  void ShowBlackJack();
  void ShowShop();
  void ShowChooseGame();
  void ShowFruitMachine();

  void StopAllSounds(QMediaPlayer* except = nullptr);

 private:
  View();

  void InitView();
  void InitGameScene();
  void InitMainMenu();
  void InitChooseGame();
  void InitSettings();
  void InitCasino();
  void InitBlackJack();
  void InitFruitMachine();
  void InitShop();
  void ManageSounds();

  static QLabel* QLabelOrientate(const QString& text, Qt::Alignment);
  static void DisableScrollbars(QGraphicsView* graphics);
  void LoadBackgroundFrames(const QString& folder);

  // Global
  QGraphicsView* view_{};

  // Game
  QGraphicsScene* game_scene_{};
  QPushButton* permit_button_{};
  QPushButton* reject_button_{};
  QLabel* lives_{};
  QPushButton* game_pause_button_{};
  QLabel* guests_left_{};
  QLabel* day_{};
  QLabel* time_left_{};
  QList<QPixmap> background_frames_;
  QGraphicsPixmapItem* game_background_{};
  int bg_frame_index_{};

  const int32_t kFrameRate = 10;
  const int32_t kFrameDelay = 1000 / kFrameRate;

  QWidget* game_pause_overlay;
  QPushButton* game_exit_;
  QPushButton* game_continue_;

  QWidget* game_over_overlay_;
  QPushButton* to_main_menu_;
  QPushButton* to_casino_;
  QPushButton* to_shop_;
  QLabel* status_label_;

  QWidget* game_intro_overlay_;
  QLabel* intro_;

  QTimer* guests_;

  // Menu
  QGraphicsScene* menu_scene_{};
  QPushButton* start_game_{};
  QPushButton* open_settings_{};
  QPushButton* quit_{};

  // Choose game
  QGraphicsScene* choose_game_scene_{};
  QPushButton* continue_button_{};
  QPushButton* new_game_button_{};
  QPushButton* to_menu_from_choose_game_button_{};
  QPushButton* to_casino_from_choose_button_{};
  QPushButton* to_shop_from_choose_button_{};

  // Settings
  QGraphicsScene* settings_scene_{};
  QPushButton* difficulty_{};
  QPushButton* sound_{};
  QPushButton* reset_defaults_{};
  QPushButton* exit_shortcut_{};
  QPushButton* back_to_menu_{};
  QWidget* shortcut_request_overlay_{};

  // Casino
  QGraphicsScene* casino_scene_{};
  QPushButton* black_jack_{};
  QPushButton* fruit_machine_{};
  QPushButton* casino_exit_{};

  // Black Jack
  QGraphicsScene* black_jack_scene_{};
  QSpinBox* bid_{};
  QPushButton* make_bid_{};
  QPushButton* back_to_casino_{};
  QLabel* player_money_{};
  QLabel* croupier_{};
  QPushButton* hit_me_{};
  QPushButton* stand_{};
  std::vector<QLabel*> croupier_cards_{};
  std::vector<QLabel*> player_cards_{};
  QLabel* status_{};

  // Shop
  QGraphicsScene* shop_scene_;
  QPushButton* exit_shop_;
  QLabel* shop_money_;
  std::vector<std::vector<QPushButton*>> shelves_;
  std::vector<std::vector<QLabel*>> items_;

  // Fruit machine
  QGraphicsScene* fruit_machine_scene_{};
  QPushButton* exit_fruit_machine_{};
  QSpinBox* bid_fruit_machine_{};
  QPushButton* make_bid_fruit_machine_{};
  QLabel* money_{};
  QLabel* machine_{};
  QLabel* all_slots_border_{};
  QLabel* slots_[kSlotsCount]{};
  QLabel* slots_borders_[kSlotsCount]{};

  // All sound effects
  QMediaPlayer* gachi_level_sound_;
  QMediaPlayer* menu_sound_;
  QMediaPlayer* settings_sound_;
  QMediaPlayer* casino_sound_;
  QMediaPlayer* shop_sound_;
  QMediaPlayer* bsu_level_sound_;
};
