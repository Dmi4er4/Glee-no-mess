#pragma once

#include <QKeyEvent>
#include <QPushButton>

#include <memory>

class View;
class Model;

class Controller {
 public:
  static void Init();
  static Controller& Instance();
  static bool HasInstance() { return has_instance_; }

  void keyPressEvent(QKeyEvent* event);

 private:
  Controller();
  void ConnectSignals();

  static inline bool has_instance_{false};
};
