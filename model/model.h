#pragma once

#include <QKeySequence>
#include <QObject>
#include <QSettings>
#include <QShortcut>

#include <deque>
#include <memory>
#include <vector>

#include "all_items.h"
#include "guest.h"
#include "settings.h"

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
    for (const auto& item : all_items) {
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

  void ToggleSound();

  // Difficulty
  QString GetDifficulty() {
    return settings_->value(kDifficulty).toString();
  }

  void ChangeDifficulty();

  // Shortcut
  void SetExitShortcut(const QString& keys);
  QShortcut* GetExitShortcut() { return exit_shortcut_; }

  void SetDefaultSettings();

 private:
  Model();

  void SetStartSettings();
  void SetStartSettings(const QJsonDocument&, const QString&);
  void UpdateDifficultySettings();

  size_t errors_count_{0};
  size_t time_left_{0};
  bool was_added_time_{false};
  bool is_first_mistake_{true};

  size_t errors_limit_;
  size_t guest_limit_;
  size_t time_limit_;

  std::deque<std::unique_ptr<Guest>> queue_;
  std::unique_ptr<Guest> current_guest_;
  std::vector<std::unique_ptr<Item>> all_items;

  QShortcut* exit_shortcut_;
  QSettings* settings_;
};
