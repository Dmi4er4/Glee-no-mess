#pragma once

#include "guest.h"
#include "mvc.h"

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

 private:
  int errors_{};
  friend class Controller;

  std::deque<Guest*> queue_;
  Guest* current_;

  Controller* controller_;
  View* view_{};
};
