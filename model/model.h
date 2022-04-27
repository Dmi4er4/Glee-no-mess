#pragma once

#include "guest.h"
#include "item.h"
#include "ignore_first_mistake_item.h"
#include "time_item.h"

#include <QObject>

#include <deque>
#include <memory>
#include <vector>

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

  static bool HasItem(const QString& name);

 private:
  Model();

  static size_t errors_count;
  static size_t time_left;
  static bool was_added_time;
  static bool is_first_mistake;

  static std::vector<std::shared_ptr<Item>> all_items;

  int errors_;
  std::deque<std::unique_ptr<Guest>> queue_;
  std::unique_ptr<Guest> current_guest_;
};
