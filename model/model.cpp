#include "model.h"

#include "all_items.h"
#include "file_loader.h"
#include "view.h"
#include "settings.h"

#include <utility>

Model::Model() {
  auto& view = View::Instance();
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.emplace_back(std::make_unique<Guest>());
    queue_.back()->SetIndex(i);
  }
  current_guest_ = std::make_unique<Guest>();
  current_guest_->SetActive();
  settings_ = new QSettings("GameMasters", "Glee-no-mess");
  SetDefaultSettings();
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
  View::Instance().SetErrorsCount(++errors_);
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

void Model::SetDefaultSettings() {
  QJsonDocument file =
      FileLoader::GetFile<QJsonDocument>(kDefaultSettings);
  SetDefaultSettings(file, kMoney);
  SetDefaultSettings(file, kSound);
  SetDefaultSettings(file, kComplexity);
  size_limit_ = View::Instance().maximumSize();
}

void Model::SetDefaultSettings(const QJsonDocument& file, const QString& name) {
  if (!settings_->contains(name)) {
    settings_->setValue(name, file[name].toVariant());
  }
}

void Model::SetComplexitySettings() {
  QJsonDocument file;
  QString complexity = settings_->value(kComplexity).toString();
  if (complexity == kEasy) {
    file = FileLoader::GetFile<QJsonDocument>(kEasySettings);
  } else if (complexity == kMedium) {
    file = FileLoader::GetFile<QJsonDocument>(kMediumSettings);
  } else {
    file = FileLoader::GetFile<QJsonDocument>(kHardSettings);
  }
  errors_limit_ = file[kErrorsCount].toInt();
  guest_limit_ = file[kGuestCount].toInt();
  time_limit_ = file[kTime].toInt();
}
