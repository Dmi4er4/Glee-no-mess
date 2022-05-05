#pragma once

#include <QApplication>
#include <QKeyEvent>
#include <QPushButton>

#include <memory>

class View;
class Model;

class Controller {
 public:
  static Controller& Instance();

  void keyPressEvent(QKeyEvent* event);
  void KeyPressInSetting(QKeyEvent* event);
  void KeyPressInGame(QKeyEvent* event);
  void ConnectShortcutSignals();

 private:
  Controller();

  void ConnectSignals();
  void ConnectGameSignals();
  void ConnectSettingsMenuSignals();
  void ConnectMainMenuSignals();

  QString PressedKey(Qt::KeyboardModifier key, const QString& text) {
    return QApplication::keyboardModifiers() & key ? text : "";
  }
};
