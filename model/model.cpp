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
  SetStartSettings();
  SetComplexitySettings();
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
  for (auto item : all_items) {
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
  for (auto item : all_items) {
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

void Model::SetStartSettings() {
  QJsonDocument file =
      FileLoader::GetFile<QJsonDocument>(kDefaultSettings);
  SetStartSettings(file, kMoney);
  SetStartSettings(file, kSound);
  SetStartSettings(file, kComplexity);
  SetStartSettings(file, kExitShortcut);
  View::Instance().SetComplexityButton(
      settings_->value(kComplexity).toString());
  View::Instance().SetExitShortcut(
      kExitShortcutText + settings_->value(kExitShortcut).toString());
  View::Instance().SetSound(settings_->value(kSound).toString());
  exit_shortcut_ = new QShortcut(
      QKeySequence(settings_->value(kExitShortcut).toString()),
      &View::Instance());
}

void Model::SetStartSettings(const QJsonDocument& file, const QString& name) {
  if (!settings_->contains(name)) {
    settings_->setValue(name, file[name]);
  }
}

void Model::SetComplexitySettings() {
  auto file = FileLoader::GetFile<QJsonDocument>(kComplexitySettings);
  auto complexity = settings_->value(kComplexity).toString();
  errors_limit_ = file[complexity][kErrorsCount].toInt();
  guest_limit_ = file[complexity][kGuestCount].toInt();
  time_limit_ = file[complexity][kTime].toInt();
}

void Model::ChangeComplexity() {
  if (settings_->value(kComplexity) == kEasy) {
    settings_->setValue(kComplexity, kMedium);
  } else if (settings_->value(kComplexity) == kMedium) {
    settings_->setValue(kComplexity, kHard);
  } else {
    settings_->setValue(kComplexity, kEasy);
  }
  View::Instance().SetComplexityButton(
      settings_->value(kComplexity).toString());
  SetComplexitySettings();
}

void Model::SetExitShortcut(const QString& keys) {
  settings_->setValue(kExitShortcut, keys);
  exit_shortcut_->setKey(keys);
}

void Model::ChangeSoundStatus() {
  if (settings_->value(kSound) == kOff) {
    settings_->setValue(kSound, kOn);
  } else {
    settings_->setValue(kSound, kOff);
  }
  View::Instance().SetSound(settings_->value(kSound).toString());
}

void Model::SetDefaultSettings() {
  settings_->setValue(kComplexity, kEasy);
  settings_->setValue(kExitShortcut, kDefaultExitShortcut);
  settings_->setValue(kSound, kOn);
  View::Instance().SetComplexityButton(kEasy);
  View::Instance().SetExitShortcut(kExitShortcutText + kDefaultExitShortcut);
  View::Instance().SetSound(kOn);
  exit_shortcut_->setKey(QKeySequence(kDefaultExitShortcut));
}
