#pragma once

#include <QFile>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "file_loader.h"
#include "guest.h"
#include "typedefs.h"
#include "settings.h"

class Level {
 public:
  Level(const QString& level_name, const QString& difficulty);

  void InitState(int day = 0);
  void GenerateGuests();
  void SetDay(int day);
  bool NextDay();
  auto GetCurrentDay() { return days_[state_.current_day_]; }
  void AddTime(int time);
  void DecrementTimeLeft() { --state_.seconds_left_; }
  void IncrementMistakes() { ++state_.errors_count_; }
  bool DayFailed() { return state_.errors_count_ >= errors_max_; }
  Guest* GetNextGuest();
  void ShiftNextGuest();

  auto GetTimeLeft() { return state_.seconds_left_; }
  auto GetLives() { return errors_max_ - state_.errors_count_; }
  auto GetDayIndex() { return state_.current_day_; }
  auto GetGuestsLeft() { return state_.current_day_; }
  auto GetQueueThreshold() { return queue_threshold_millis_; }
  auto GetPath() { return path_; }

 private:
  struct Day {
    QString intro_;
    QSet<QString> allowed_dudes_;
    int reward_;
    int penalty_;
  };
  struct State {
    int current_day_;
    int errors_count_;
    int seconds_left_;
    int guests_left_;
    bool was_added_time_;
    int next_guest_index_;
  };

  QString path_;
  QString level_name_;
  QString level_intro_;
  int errors_max_{};
  int duration_seconds_{};
  int guests_max_{};
  std::vector<QString> traits_;
  std::vector<QString> dudes_;
  int queue_threshold_millis_;

  std::vector<Day> days_;
  State state_{};
  std::vector<std::unique_ptr<Guest>> guests_;
};
