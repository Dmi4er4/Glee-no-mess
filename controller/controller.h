#pragma once

#include <QKeyEvent>
#include <QPushButton>

#include <memory>

class View;
class Model;

class Controller {
 public:
  static Controller& Instance();

  void keyPressEvent(QKeyEvent* event);

  void ConnectShortcutSignals();

 private:
  Controller();

  void ConnectSignals();
  void ConnectGameSignals();
  void ConnectSettingsMenuSignals();
  void ConnectMainMenuSignals();
};
