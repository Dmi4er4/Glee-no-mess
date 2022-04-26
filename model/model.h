#pragma once

#include "guest.h"
#include "items.h"
#include "ignore_first_mistake_item.h"
#include "time_item.h"

#include <QObject>

#include <deque>
#include <memory>

class View;
class Controller;

class Model : public QObject {
 public:
  static Model& Instance();

  static constexpr int kQueueLength = 3;

  void Permit();
  void Reject();
  void ShiftQueue();
  void IncreaseErrorsCount();

  static void StartNewLevel();

  static void AddIgnoreFirstMistakeItem();
  static void AddTimeItem();

  static void UpdateMistake();
  static void UpdateTimeLeft();

  static void ForgiveFirstMistake();
  static void AddTime(size_t time);

 private:
  Model();

  int errors_;
  static size_t errors_count_;
  static size_t time_left_;

  std::deque<std::unique_ptr<Guest>> queue_;
  std::unique_ptr<Guest> current_guest_;

  static bool was_added_time_;
  static bool is_first_mistake_;
  static std::vector<Items*> all_items_;
};
