#pragma once

#include <QKeySequence>
#include <QObject>
#include <QSettings>
#include <QShortcut>
#include <QTimer>

#include <algorithm>
#include <deque>
#include <memory>
#include <vector>

#include "all_items.h"
#include "black_jack.h"
#include "guest.h"
#include "level.h"
#include "settings.h"

class View;
class Controller;

class Model : public QObject {
 public:
  static Model& Instance();

  static constexpr size_t kQueueLength = 3;

  void Permit();
  void Reject();
  void ShiftQueue();
  void IncreaseErrorsCount();

  void StartNewDay();

  // Items

  void AddIgnoreFirstMistakeItem();
  void AddTimeItem();
  void AddAllInItem();
  void AddReduceGuestsItem();

  // TODO(Adamenko-Vladislav) Items must do something
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

  static constexpr int32_t kCost = 1000;

  void BuyAddIgnoreFirstMistakeItem();
  void BuyTimeItem();
  void BuyAllInItem();
  void BuyReduceGuestsItem();

  // Sound
  bool IsSoundOn() {
    return settings_->value(kSound).toString() == kOn;
  }

  void ToggleSound();

  // Difficulty
  auto GetDifficulty() {
    return settings_->value(kDifficulty).toString();
  }

  void ChangeDifficulty();

  // Shortcut
  void SetExitShortcut(const QString& keys);
  QShortcut* GetExitShortcut() { return exit_shortcut_; }

  // Settings
  void ResetDefaults();

  // Black Jack
  void StartNewGameBlackJack();

  // Money
  void UpdateMoney(int delta);

  // Fruit Machine
  void StartFruitMachineGame();
  size_t GetGuestsLeft() const { return guest_left_; }
  QString GetTimeLeft() const;
  QTimer* GetDayTimer() const { return day_timer_; }

  void ConnectSignals();

  int LoadSettingsDay() const;
  void UpdateSettingsDay(int new_day);

  QString LoadSettingsLevel() const;
  void UpdateSettingsLevel(const QString& new_level);

  void DayPassed();
  void DayFailed();

 private:
  Model();

  void InitSettings();
  void InitSettings(const QJsonDocument&, const QString&);
  void UpdateDifficultySettings();

  size_t errors_count_{0};
  size_t time_left_{0};
  size_t guest_left_{0};
  bool was_added_time_{false};
  bool is_first_mistake_{true};

  size_t errors_limit_;
  size_t guest_limit_;
  size_t time_limit_;

  Level level_{kLevels, "club_level", 0};

  std::deque<std::shared_ptr<Guest>> queue_;
  std::shared_ptr<Guest> current_guest_;
  std::vector<std::shared_ptr<Item>> all_items;

  QShortcut* exit_shortcut_;
  QSettings* settings_;
  QTimer* day_timer_;

  // Fruit Machine
  int spinning_;
  QTimer* slot_update_timer_;
  int new_slot_pics_[3];
};
