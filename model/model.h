#pragma once

#include "guest.h"

#include <QObject>

#include <deque>

class View;
class Controller;

class Model : public QObject {
 public:
  static Model& Instance();

  static constexpr int kQueueLength = 3;

  void Paint();
  void Permit();
  void Reject();
  void ShiftQueue();

  void UpdateErrors();

 private:
  Model();

  int errors_{};

  std::deque<Guest*> queue_;
  Guest* current_;
};
