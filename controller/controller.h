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

  void ConnectShortcutSignals(View& view, Model& model);
  void DisconnectShortcutSignals(View& view, Model& model);

 private:

  Controller();

  void ConnectSignals();
  void ConnectGameSignals(View& view, Model& model);
  void ConnectSettingsMenuSignals(View& view, Model& model);
  void ConnectMainMenuSignals(View& view, Model& model);
};
