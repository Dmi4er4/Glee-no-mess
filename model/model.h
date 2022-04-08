#pragma once

#include "guest.h"

#include <QObject>

#include <deque>
#include <memory>

class View;
class Controller;

class Model : public QObject {
 public:
  static Model& Instance();

  static constexpr int kQueueLength = 3;

  static void Paint();
  static void Permit();
  static void Reject();
  static void ShiftQueue();
  static void IncreaseErrorsCount();

 private:
  Model();

  int errors_{};

  std::deque<std::unique_ptr<Guest>> queue_;
  std::unique_ptr<Guest> current_guest_;
};
