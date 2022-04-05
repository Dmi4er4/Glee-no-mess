#pragma once

#include <deque>
#include <QObject>
#include "guest/guest.h"

class View;
class Controller;

class Model : public QObject {
 public:
  explicit Model(Controller* controller);
  void Paint();
  void Permit();
  void Reject();
  void ShiftQueue();
  void UpdateErrors();

  std::deque<Guest*> queue_;
  Guest* current_;
  int errors_{};

  static constexpr int kQueueLength = 3;

 private:
  friend class Controller;

  Controller* controller_;
  View* view_{};
};
