#pragma once

#include <QKeyEvent>
#include <QPushButton>

#include <memory>

class View;
class Model;

class Controller {
 public:
  static Controller& Instance();

  static void keyPressEvent(QKeyEvent* event);

 private:
  Controller();
  void ConnectSignals();
};
