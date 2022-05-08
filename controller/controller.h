#pragma once

#include <QApplication>
#include <QKeyEvent>
#include <QPushButton>

#include <memory>

class View;
class Model;

class Controller : public QObject {
 public:
  static Controller& Instance();

  void keyPressEvent(QKeyEvent* event);
  void KeyPressInSettings(QKeyEvent* event);
  void KeyPressInGame(QKeyEvent* event);
  void ConnectShortcutSignals();

  bool eventFilter(QObject *obj, QEvent *event) override;

 private:
  Controller();

  void ConnectSignals();
  void ConnectGameSignals();
  void ConnectSettingsMenuSignals();
  void ConnectMainMenuSignals();
  void ConnectChooseGameSignals();
  void Quit();

  QString PressedKey(Qt::KeyboardModifier key, const QString& text) {
    return QApplication::keyboardModifiers() & key ? text : "";
  }

  bool is_editing_keybinding_{};
  std::function<void(const QString& keys)> new_hotkey_;
};
