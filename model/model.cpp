#include "model.h"

#include <utility>

#include "file_loader.h"
#include "view.h"

Model::Model() {
  auto& view = View::Instance();
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.emplace_back(std::make_unique<Guest>());
    queue_.back()->SetIndex(i);
  }
  current_guest_ = std::make_unique<Guest>();
  current_guest_->SetActive();
  settings_ = new QSettings("GameMasters", "Glee-no-mess");
  InitSettings();
  UpdateDifficultySettings();
}

void Model::Permit() {
  if (!Instance().current_guest_->IsMale()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
}

void Model::Reject() {
  if (Instance().current_guest_->IsMale()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
}

void Model::ShiftQueue() {
  current_guest_ = std::move(queue_.front());
  queue_.pop_front();
  queue_.emplace_back(std::make_unique<Guest>());
  for (int i = 0; i < queue_.size(); ++i) {
    queue_[i]->SetIndex(i);
  }
  current_guest_->SetActive();
}

void Model::IncreaseErrorsCount() {
  View::Instance().SetErrorsCount(++errors_count_);
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

void Model::StartNewLevel() {
  errors_count_ = 0;
  is_first_mistake_ = true;
  // time_lest =
  was_added_time_ = false;
  // TODO(Adamenko-Vladislav)
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
  settings_->setValue(kDifficulty, kEasy);
  settings_->setValue(kExitShortcut, kDefaultExitShortcut);
  settings_->setValue(kSound, kOn);
  View::Instance().SetDifficulty(kEasy);
  View::Instance().SetExitShortcut(kDefaultExitShortcut);
  View::Instance().SetSound(kOn);
  exit_shortcut_->setKey(QKeySequence(kDefaultExitShortcut));
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
}
