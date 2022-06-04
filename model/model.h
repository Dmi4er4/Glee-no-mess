#pragma once

#include <QJsonDocument>
#include <QKeySequence>
#include <QObject>
#include <QSettings>
#include <QShortcut>
#include <QTimer>

#include <algorithm>
#include <deque>
#include <queue>
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

  void Permit();
  void Reject();
  void IncreaseErrorsCount();
  QString LoadSettingsLevel();
  QString LoadSettingsDay();
  QJsonObject CurrentSave();
  void DoAnimation(int millis);
  void StartDay();
  void StartNewGame();
  void ContinueGame();
  void DayFailed();
  void DayPassed();
  bool TryLoadSave();
  void ShiftQueue();
  void DayTimerTick();

  auto GetLevel() { return level_.get(); }
  auto GetDayTimer() { return day_timer_.get(); }

  // Items
  void AddIgnoreFirstMistakeItem();
  void AddTimeItem();
  void AddAllInItem();
  void AddReduceGuestsItem();

  void AddTime(size_t time);
  bool HasItem(const QString& name);

  static constexpr int32_t kCost = 300;

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
  void UpdateMoney(money_t delta);

  // Fruit Machine
  void StartFruitMachineGame();

 private:
  Model();

  void ReadLevels();
  void SaveGame(const QString& level_name, int day_index);
  void InitSettings();
  void InitSetting(const QJsonDocument& json, const QString& property);

  std::vector<std::unique_ptr<Item>> all_items_;

  QShortcut* exit_shortcut_;
  QSettings* settings_;

  // Level
  std::unique_ptr<Level> level_;
  std::vector<QString> level_names_;
  int current_level_index_{};
  std::deque<Guest*> queue_;
  std::unique_ptr<QTimer> day_timer_;

  // Fruit Machine
  int spinning_;
  QTimer* slot_update_timer_;
  int new_slot_pics_[kSlotsCount];
};
