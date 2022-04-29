#pragma once

#include "all_items.h"
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

  void StartNewLevel();
  void AddIgnoreFirstMistakeItem();
  void AddTimeItem();
  void UpdateMistake();

  void UpdateTimeLeft() {
    for (auto item : all_items) {
      item->TimeTrigger();
    }
  }

  void ForgiveFirstMistake() {
    if (is_first_mistake) {
      errors_count--;
    }
  }

  void AddTime(size_t time);
  bool HasItem(const QString& name);

 private:
  Model();

  size_t errors_count{0};
  size_t time_left{0};
  bool was_added_time{false};
  bool is_first_mistake{true};

  std::vector<std::shared_ptr<Item>> all_items{};

  int errors_;
  std::deque<std::unique_ptr<Guest>> queue_;
  std::unique_ptr<Guest> current_guest_;
};
