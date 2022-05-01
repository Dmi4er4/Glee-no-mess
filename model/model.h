#pragma once

#include "all_items.h"
#include "guest.h"
#include "item.h"
#include "ignore_first_mistake_item.h"
#include "time_item.h"
#include "settings.h"

#include <QObject>
#include <QSettings>
#include <QShortcut>
#include <QKeySequence>

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
    for (auto item: all_items) {
      item->TimeTrigger();
    }
  }

  void ForgiveFirstMistake() {
    if (is_first_mistake_) {
      errors_count_--;
    }
  }

  void AddTime(size_t time);
  bool HasItem(const QString& name);

  // Sound

  bool IsSoundOn() {
    return settings_->value(kSound).toString() == kOn;
  }

  void ChangeSoundStatus();

  // Complexity

  QString GetComplexity() {
    return settings_->value(kComplexity).toString();
  }

  void ChangeComplexity();

  // Shortcut

  void SetExitShortcut(const QString& keys);
  QShortcut* GetExitShortcut() { return exit_shortcut_; }

  void SetDefaultSettings();

 private:
  Model();

  void SetStartSettings();
  void SetStartSettings(const QJsonDocument&, const QString&);
  void SetComplexitySettings();

  size_t errors_count_{0};
  size_t time_left_{0};
  bool was_added_time_{false};
  bool is_first_mistake_{true};

  std::vector<std::shared_ptr<Item>> all_items{};

  int errors_;
  std::deque<std::unique_ptr<Guest>> queue_;
  std::unique_ptr<Guest> current_guest_;

  QSettings* settings_;

  size_t errors_limit_;
  size_t guest_limit_;
  size_t time_limit_;

  QShortcut* exit_shortcut_;
};
