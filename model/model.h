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

  static void Paint();
  static void Permit();
  static void Reject();
  static void ShiftQueue();

  static void ErrorsPlusPlus();

 private:
  Model();

  int errors_{};

  std::deque<Guest*> queue_;
  Guest* current_;
};
