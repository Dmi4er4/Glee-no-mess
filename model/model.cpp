#include "model.h"

#include <utility>

#include "file_loader.h"
#include "view.h"

Model::Model() {
  day_timer_ = new QTimer(this);
  day_timer_->setInterval(1000);

  settings_ = new QSettings("GameMasters", "Glee-no-mess");
  InitSettings();
  UpdateDifficultySettings();
  ConnectSignals();
}

void Model::Permit() {
  // TODO(Andrey-Kostianoy): Compare with bad characteristics
  if (!Instance().current_guest_->IsMale()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
}

void Model::Reject() {
  // TODO(Andrey-Kostianoy): Compare with bad characteristics
  if (Instance().current_guest_->IsMale()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
}

void Model::ShiftQueue() {
  guest_left_--;
  View::Instance().SetGuestsLeft(guest_left_);
  if (guest_left_ == 0) {
    DayPassed();
    return;
  }

  current_guest_ = queue_.front();
  queue_.pop_front();
  int current_day = LoadSettingsDay();

  if (guest_left_ > kQueueLength) {
    queue_.emplace_back(level_.GetKthGuestInDay(
        guest_limit_ - (guest_left_ - kQueueLength), current_day));
  }

  for (int i = 0; i < queue_.size(); ++i) {
    queue_[i]->SetIndex(i);
  }
  current_guest_->SetActive();
}

void Model::IncreaseErrorsCount() {
  errors_count_++;
  if (errors_count_ >= errors_limit_) {
    DayFailed();
  } else {
    View::Instance().SetErrorsCount(errors_count_);
  }
}

Model& Model::Instance() {
  static Model instance;
  return instance;
}

void Model::UpdateMistake() {
  errors_count_++;
  for (const auto& item : all_items) {
    item->MistakeTrigger();
  }
  is_first_mistake_ = false;
  // TODO(Adamenko-Vladislav)
}

void Model::StartNewDay() {
  errors_count_ = 0;
  is_first_mistake_ = true;
  time_left_ = time_limit_;
  guest_left_ = guest_limit_;
  was_added_time_ = false;

  int current_day = LoadSettingsDay();

  queue_.clear();
  for (int i = 0; i < std::min(guest_limit_ - 1, kQueueLength); ++i) {
    queue_.emplace_back(level_.GetKthGuestInDay(i + 1, current_day));
    queue_.back()->SetIndex(i);
  }
  current_guest_ = level_.GetKthGuestInDay(0, current_day);
  current_guest_->SetActive();

  day_timer_->start();
}

void Model::AddTime(size_t time) {
  if (!was_added_time_) {
    time_left_ += time;
    was_added_time_ = true;
  }
}

bool Model::HasItem(const QString& name) {
  for (const auto& item : all_items) {
    if (item->GetName() == name) {
      return true;
    }
  }
  return false;
}

void Model::AddTimeItem() {
  if (!HasItem(kAddTime)) {
    all_items.emplace_back(new TimeItem);
  }
}

void Model::AddIgnoreFirstMistakeItem() {
  if (!HasItem(kIgnoreFirstMistake)) {
    all_items.emplace_back(new IgnoreFirstMistakeItem);
  }
}

QString Model::GetTimeLeft() const {
  size_t minutes = time_left_ / kSeconds;
  size_t seconds = time_left_ % kSeconds;
  QString result;
  result += static_cast<char>('0' + minutes);
  result += ":";

  if (seconds > 9) {
    result += static_cast<char>('0' + seconds / 10);
    result += static_cast<char>('0' + seconds % 10);
  } else {
    result += '0';
    result += static_cast<char>('0' + seconds);
  }
  return result;
}

void Model::InitSettings() {
  auto file = FileLoader::GetFile<QJsonDocument>(kDefaultSettings);
  for (const auto& property_name : {kMoney,
                                    kSound,
                                    kDifficulty,
                                    kExitShortcut}) {
    InitSettings(file, property_name);
  }
  View::Instance().SetDifficulty(
      settings_->value(kDifficulty).toString());
  View::Instance().SetExitShortcut(
      settings_->value(kExitShortcut).toString());
  View::Instance().SetSound(settings_->value(kSound).toString());
  exit_shortcut_ = new QShortcut(
      QKeySequence(settings_->value(kExitShortcut).toString()),
      &View::Instance());
  View::Instance().SetPlayerMoney(settings_->value(kMoney).toInt());
  View::Instance().SetPlayerMaxBid(settings_->value(kMoney).toInt());
}

void Model::InitSettings(const QJsonDocument& json, const QString& property) {
  if (!settings_->contains(property)) {
    settings_->setValue(property, json[property]);
  }
}

void Model::UpdateDifficultySettings() {
  auto file = FileLoader::GetFile<QJsonDocument>(kDifficultySettings);
  auto difficulty = settings_->value(kDifficulty).toString();
  errors_limit_ = file[difficulty][kErrorsCount].toInt();
  guest_limit_ = file[difficulty][kGuestCount].toInt();
  time_limit_ = file[difficulty][kTime].toInt();
  level_ = Level(kLevels, "club_level", guest_limit_);
}

void Model::DayPassed() {
  // TODO(Kostianoy-Andrey): add here message for view
  day_timer_->stop();

  int current_day = LoadSettingsDay();
  if (current_day < level_.GetDays()) {
    current_day++;
  }
  UpdateSettingsDay(current_day);

  View::Instance().ShowMainMenu();
}

void Model::DayFailed() {
  // TODO(Kostianoy-Andrey): add here message for view
  day_timer_->stop();
  View::Instance().ShowMainMenu();
}

int Model::LoadSettingsDay() const {
  // TODO(Kostianoy-Andrey): format in settings "hardness_day"
  QString key = settings_->value(kDifficulty).toString() + "_day";
  if (!settings_->contains(key)) {
    settings_->setValue(key, 1);
  }
  return settings_->value(key).toInt();
}

void Model::UpdateSettingsDay(int new_day) {
  // TODO(Kostianoy-Andrey): format in settings "hardness_day"
  QString key = settings_->value(kDifficulty).toString() + "_day";
  settings_->setValue(key, new_day);
}

QString Model::LoadSettingsLevel() const {
  // TODO(Kostianoy-Andrey): format in settings "hardness_level"
  QString key = settings_->value(kDifficulty).toString() + "_level";
  if (!settings_->contains(key)) {
    QJsonDocument source = FileLoader::GetFile<QJsonDocument>(kLevels);
    settings_->setValue(key, source["club_level"]["name"]);
  }
  return settings_->value(key).toString();
}

void Model::UpdateSettingsLevel(const QString& new_level) {
  // TODO(Kostianoy-Andrey): format in settings "hardness_level"
  QString key = settings_->value(kDifficulty).toString() + "_level";
  settings_->setValue(key, new_level);
}

void Model::ChangeDifficulty() {
  if (settings_->value(kDifficulty) == kEasy) {
    settings_->setValue(kDifficulty, kMedium);
  } else if (settings_->value(kDifficulty) == kMedium) {
    settings_->setValue(kDifficulty, kHard);
  } else {
    settings_->setValue(kDifficulty, kEasy);
  }
  View::Instance().SetDifficulty(
      settings_->value(kDifficulty).toString());
  UpdateDifficultySettings();
}

void Model::SetExitShortcut(const QString& keys) {
  settings_->setValue(kExitShortcut, keys);
  exit_shortcut_->setKey(keys);
  View::Instance().SetExitShortcut(keys);
}

void Model::ToggleSound() {
  if (settings_->value(kSound) == kOff) {
    settings_->setValue(kSound, kOn);
  } else {
    settings_->setValue(kSound, kOff);
  }
  View::Instance().SetSound(settings_->value(kSound).toString());
}

void Model::ResetDefaults() {
  auto default_settings =
      FileLoader::GetFile<QJsonDocument>(kDefaultSettings);
  const QString default_sound = default_settings[kSound].toString();
  const QString default_difficulty = default_settings[kDifficulty].toString();
  const QString default_exit_shortcut =
      default_settings[kExitShortcut].toString();

  settings_->setValue(kDifficulty, default_difficulty);
  settings_->setValue(kExitShortcut, default_exit_shortcut);
  settings_->setValue(kSound, default_sound);

  View::Instance().SetDifficulty(default_difficulty);
  View::Instance().SetExitShortcut(default_exit_shortcut);
  View::Instance().SetSound(default_sound);
  exit_shortcut_->setKey(QKeySequence(default_exit_shortcut));
  UpdateDifficultySettings();
}

void Model::ConnectSignals() {
  connect(day_timer_, &QTimer::timeout,
          this, [this] () {
            time_left_--;
            QString new_visible_time = GetTimeLeft();
            View::Instance().SetTimeLeft(new_visible_time);

            if (time_left_ == 0) {
              DayFailed();
            }
          });
}

void Model::StartNewGameBlackJack() {
  auto bid = View::Instance().GetBid();
  if (bid == 0) {
    return;
  }
  View::Instance().ShowBlackJackGame();
  View::Instance().ShowSatus("");
  UpdateMoney(-bid);
  BlackJack::Instance().StartNewGame(bid);
}

void Model::UpdateMoney(int delta) {
  money_t money = settings_->value(kMoney).toInt();
  money += delta;
  settings_->setValue(kMoney, QString::number(money));
  View::Instance().SetPlayerMoney(money);
  View::Instance().SetPlayerMaxBid(money);
}

void Model::StartFruitMachineGame() {
  auto bid = View::Instance().GetFruitMachineBid();
  if (bid == 0) {
    return;
  }
  UpdateMoney(-bid);
  auto slot0 = Random::RandomInt(0, 8);
  auto slot1 = Random::RandomInt(0, 8);
  auto slot2 = Random::RandomInt(0, 8);

  static const QString path = ":casino/machine_";

  View::Instance().SetSlot0(FileLoader::GetFile<QPixmap>(
      path + QString::number(slot0) + ".png"));
  View::Instance().SetSlot1(FileLoader::GetFile<QPixmap>(
      path + QString::number(slot1) + ".png"));
  View::Instance().SetSlot2(FileLoader::GetFile<QPixmap>(
      path + QString::number(slot2) + ".png"));
  if (slot0 == slot1 && slot0 == slot2) {
    UpdateMoney(5 * bid);
  }
  if (slot0 == slot1 || slot0 == slot2 || slot1 == slot2) {
    UpdateMoney(2 * bid);
  }
}
