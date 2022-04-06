#pragma once

#include "guest/guest.h"

#include <QObject>

#include <deque>

class View;
class Controller;

class Model : public QObject {
 public:
  static constexpr int kQueueLength = 3;

  explicit Model(Controller* controller);
  void Paint();
  void Permit();
  void Reject();
  void ShiftQueue();

  void UpdateErrors();
  int errors_{};

 private:
  friend class Controller;

  std::deque<Guest*> queue_;
  Guest* current_;

  Controller* controller_;
  View* view_{};
};
