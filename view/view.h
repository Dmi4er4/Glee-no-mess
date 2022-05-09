#pragma once

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>
#include <QFormLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QStyle>
#include <QTimer>
#include <QTextEdit>

#include "guest.h"
#include "typedefs.h"

class Model;
class Controller;

class View : public QMainWindow {
  Q_OBJECT

 public:
  static View& Instance();
  static constexpr int kBlue = 0x0dc1fb;
  static constexpr int kYellow = 0xfdff73;
  static constexpr int kSquareSize = 80;
  static constexpr int kMargin = 5;
  static inline const QBrush kBluePolygonBrush = QBrush(QColor(kBlue));
  static inline const QBrush kYellowPolygonBrush = QBrush(QColor(kYellow));

  void keyPressEvent(QKeyEvent* event) override;

  // Game
  bool IsGame() const { return view_->scene() == game_scene_; }

  void SetErrorsCount(int value) {
    errors_->setText("Errors: " + QString::number(value));
  }

  void SetTimer();
  void ChangeFrame();

  auto GetPermitButton() const { return permit_button_; }
  auto GetRejectButton() const { return reject_button_; }
  auto GetScene() const { return game_scene_; }

  // Menu
  bool IsMenu() const { return view_->scene() == menu_scene_; }
  auto GetOpenSettingsButton() const { return open_settings_; }
  auto GetStartGameButton() const { return start_game_; }

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

  void SetPlayerMoney(money_t x) {
    player_money_->setText("Your money: " + QString::number(x));
  }

  void SetPlayerMaxBid(money_t x) {
    bid_->setMaximum(x);
  }

  auto* GetMakeBidButton() { return make_bid_; }
  auto GetBid() { return bid_->value(); }
  auto* GetHitMeButton() { return hit_me_; }
  auto* GetStandButton() { return stand_; }

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
  }

  void CloseBlackJackGame() {
    hit_me_->close();
    stand_->close();
  }

  void PutCroupierCard(size_t number, QPixmap card) {
    croupier_cards_[number]->setPixmap(card);
  }

  void PutPlayerCard(size_t number, QPixmap card) {
    player_cards_ [number]->setPixmap(card);
  }

  void ShowSatus(const QString& status) {
    status_->setText(status);
    CloseBlackJackGame();
  }

  // Show Scene
  void ShowGame() { view_->setScene(game_scene_); }
  void ShowMainMenu() { view_->setScene(menu_scene_); }
  void ShowSettings() { view_->setScene(settings_scene_); }

  void ShowCasino() {
    view_->setScene(casino_scene_);
  }

  void ShowBlackJack() {
    CloseBlackJackGame();
    for (auto to : croupier_cards_) {
      to->setPixmap(QPixmap());
    }
    for (auto to : player_cards_) {
      to->setPixmap(QPixmap());
    }
    status_->setText("");
    view_->setScene(black_jack_scene_);
  }
  
 private:
  View();

  void InitView();
  void InitGameScene();
  void InitMainMenu();
  void InitSettings();
  void InitCasino();
  void InitBlackJack();
  static QLabel* QLabelOrientate(const QString& text, Qt::Alignment);
  static void DisableScrollbars(QGraphicsView* graphics);
  void LoadBackgroundFrames(const QString& folder);

  // Global
  QGraphicsView* view_{};

  // Game
  QGraphicsScene* game_scene_{};
  QPushButton* permit_button_{};
  QPushButton* reject_button_{};
  QLabel* errors_{};
  QList<QPixmap> background_frames_;
  QGraphicsPixmapItem* game_background_{};
  int bg_frame_index_{};

  const int32_t kFrameRate = 10;

  // Menu
  QGraphicsScene* menu_scene_{};
  QPushButton* start_game_{};
  QPushButton* open_settings_{};
  QPushButton* quit_{};

  // Settings
  QGraphicsScene* settings_scene_{};
  QPushButton* difficulty_{};
  QPushButton* sound_{};
  QPushButton* reset_defaults_{};
  QPushButton* exit_shortcut_{};
  QPushButton* back_to_menu_{};
  QWidget* shortcut_request_overlay_{};

  // BlackJack
  QGraphicsScene* casino_scene_{};
  QPushButton* black_jack_{};
  QPushButton* fruit_machine_{};
  QPushButton* casino_exit_{};

  // Black Jack
  QGraphicsScene* black_jack_scene_;
  QSpinBox* bid_;
  QPushButton* make_bid_;
  QPushButton* back_to_casino_;
  QLabel* player_money_;
  QLabel* croupier_;
  QPushButton* hit_me_{};
  QPushButton* stand_{};
  std::vector<QLabel*> croupier_cards_{};
  std::vector<QLabel*> player_cards_{};
  QLabel* status_;
};
