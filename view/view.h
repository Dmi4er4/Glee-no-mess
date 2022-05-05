#pragma once

#include <QApplication>
#include <QDirIterator>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

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
  bool IsGame() const { return centralWidget() == game_graphics_; }

  void SetErrorsCount(int value) {
    errors_->setText("Errors: " + QString::number(value));
  }

  void SetTimer();
  void SetBackgroundImage(QGraphicsView*, const QPixmap&);
  void ChangeFrame();

  auto GetPermitButton() const { return permit_button_; }
  auto GetRejectButton() const { return reject_button_; }
  auto GetScene() const { return game_scene_; }

  // Menu
  bool IsMenu() const { return centralWidget() == menu_graphics_; }
  auto GetOpenSettingsButton() const { return open_settings_; }
  auto GetStartGameButton() const { return start_game_; }

  // Settings
  bool IsSettings() const { return centralWidget() == settings_graphics_; }
  void SetExitShortcut(const QString& text) { exit_shortcut_->setText(text); }
  void SetSound(const QString& text) { sound_->setText("Sound: " + text); }

  void SetDifficultyButton(const QString& text) {
    difficulty_->setText("Difficulty: " + text);
  }

  bool IsCursorOnExitShortcut() const {
    return exit_shortcut_->geometry().contains(
        cursor().pos().x(), cursor().pos().y());
  }

  QPushButton* GetExitSettingsButton() { return exit_settings_; }
  QPushButton* GetDifficultyButton() { return difficulty_; }
  QPushButton* GetSoundButton() { return sound_; }
  QPushButton* GetDefaultSettingsButton() { return default_settings_; }

  // Show Scene
  void ShowGame();
  void ShowMainMenu();
  void ShowSettings();

  // Central Widget
  QWidget* GetCentralWidget() { return centralWidget(); }

 private:
  View();

  void CustomizeGameScene();
  void CustomizeMainMenu();
  void CustomizeSettings();
  void DisableScrollbars(QGraphicsView* graphics);
  void LoadBackgroundFrames(const QString& folder);

  // Game
  QGraphicsScene* game_scene_;
  QGraphicsView* game_graphics_;
  QPushButton* permit_button_;
  QPushButton* reject_button_;
  QLabel* errors_;
  QGraphicsProxyWidget* proxy_permit_;
  QGraphicsProxyWidget* proxy_reject_;
  QGraphicsProxyWidget* proxy_errors_;
  QList<QPixmap> background_frames_;
  QGraphicsPixmapItem* game_background_;
  int bg_frame_index_;

  const int32_t kFrameRate = 13;

  // Menu
  QGraphicsScene* menu_scene_;
  QGraphicsView* menu_graphics_;
  QPushButton* start_game_;
  QPushButton* open_settings_;
  QGraphicsProxyWidget* proxy_stat_game_;
  QGraphicsProxyWidget* proxy_open_settings_;

  // Settings
  QGraphicsScene* settings_scene_;
  QGraphicsView* settings_graphics_;
  QPushButton* exit_settings_;
  QPushButton* difficulty_;
  QPushButton* sound_;
  QPushButton* default_settings_;
  QLabel* exit_shortcut_;
  QGraphicsProxyWidget* proxy_exit_settings_;
  QGraphicsProxyWidget* proxy_difficulty_;
  QGraphicsProxyWidget* proxy_exit_shortcut_;
  QGraphicsProxyWidget* proxy_sound_;
  QGraphicsProxyWidget* proxy_default_settings_;
};
