#pragma once

#include <QString>

// Sound
const QString kSound = "sound";
const QString kOn = "on";
const QString kOff = "off";
// Money
const QString kMoney = "money";
// kDifficulty
const QString kDifficulty = "difficulty";
const QString kEasy = "easy";
const QString kHard = "hard";
const QString kMedium = "medium";
const QString kDefaultSettings = ":/settings/default_settings.json";
// Errors
const QString kErrorsCount = "errors_count";
// Guest
const QString kGuestCount = "guests_count";
// Time
const QString kTime = "time";
const size_t kSeconds = 60;
// Shortcut
const QString kExitShortcut = "exit_shortcut";
const QString kExitShortcutText = "Exit shortcut: ";
// Levels
const QString kLevels = ":/levels/levels.json";
// Fruit Machine
static constexpr int kSlotsCount = 3;
static constexpr int kBigWinIndex = 7;
static constexpr int kBigWinRewardCoef = 105;
static constexpr int kAllEqualRewardCoef = 42;
static constexpr int kAnyEqualRewardCoef = 5;
