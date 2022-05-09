#pragma once

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>
#include <QFormLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSizePolicy>
#include <QStyle>
#include <QSpacerItem>
#include <QTimer>
#include <QVBoxLayout>

#include "guest.h"

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

  void SetErrorsCount(size_t value) {
    errors_->setText("Errors: " + QString::number(value));
  }
  void SetGuestsLeft(size_t value) {
    guests_left_->setText("Visitors: " + QString::number(value));
  }
  void SetDay(size_t value) {
    day_->setText("Day: " + QString::number(value));
  }
  void SetTimeLeft(QString value) {
    time_left_->setText("Time: " + value);
  }

  void SetTimer();
  void ChangeFrame();

  auto GetPermitButton() const { return permit_button_; }
  auto GetRejectButton() const { return reject_button_; }
  auto GetToMenuFromGameButton() const { return to_menu_from_game_;  }
  auto GetScene() const { return game_scene_; }

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

  // Show Scene
  void ShowGame();
  void ShowMainMenu();
  void ShowChooseGame();
  void ShowSettings();

  // Central Widget
  QWidget* GetCentralWidget() { return centralWidget(); }

 private:
  View();

  void InitView();
  void InitGameScene();
  void InitMainMenu();
  void InitChooseGame();
  void InitSettings();
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
  QPushButton* to_menu_from_game_{};
  QLabel* guests_left_{};
  QLabel* day_{};
  QLabel* time_left_{};
  QList<QPixmap> background_frames_;
  QGraphicsPixmapItem* game_background_{};
  int bg_frame_index_{};

  const int32_t kFrameRate = 10;

  // Menu
  QGraphicsScene* menu_scene_{};
  QPushButton* start_game_{};
  QPushButton* open_settings_{};
  QPushButton* quit_{};

  // Choose game
  QGraphicsScene* choose_game_scene_;
  QPushButton* continue_button_;
  QPushButton* new_game_button_;
  QPushButton* to_menu_from_choose_game_button_;

  // Settings
  QGraphicsScene* settings_scene_{};
  QPushButton* difficulty_{};
  QPushButton* sound_{};
  QPushButton* reset_defaults_{};
  QPushButton* exit_shortcut_{};
  QPushButton* back_to_menu_{};
  QWidget* shortcut_request_overlay_{};
};
