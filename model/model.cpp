#include "model.h"

#include "all_items.h"
#include "file_loader.h"
#include "view.h"

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
  View::Instance().SetErrorsCount(++errors_);
}

Model& Model::Instance() {
  static Model instance;
  return instance;
}

void Model::UpdateMistake() {
  errors_count_++;
  for (auto item: all_items) {
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
  for (auto item: all_items) {
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
      settings_->value(kComplexity).toString()
  );
  View::Instance().SetExitShortcut(
      "Exit shortcut: " + settings_->value(kExitShortcut).toString()
  );
  View::Instance().SetSound(settings_->value(kSound).toString());
  exit_shortcut_ = new QShortcut(
      QKeySequence(settings_->value(kExitShortcut).toString()),
      &View::Instance()
  );
}

void Model::SetStartSettings(const QJsonDocument& file, const QString& name) {
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

void Model::ChangeComplexity() {
  if (settings_->value(kComplexity).toString() == kEasy) {
    settings_->setValue(kComplexity, kMedium);
  } else if (settings_->value(kComplexity).toString() == kMedium) {
    settings_->setValue(kComplexity, kHard);
  } else {
    settings_->setValue(kComplexity, kEasy);
  }
  View::Instance().SetComplexityButton(
      settings_->value(kComplexity).toString()
  );
  SetComplexitySettings();
}

void Model::SetExitShortcut(const QString& keys) {
  auto controller = Controller::Instance();
  controller.DisconnectShortcutSignals(View::Instance(), Model::Instance());
  settings_->setValue(kExitShortcut, keys);
  exit_shortcut_ = new QShortcut(QKeySequence(keys), &View::Instance());
  controller.ConnectShortcutSignals(View::Instance(), Model::Instance());
}

void Model::ChangeSoundStatus() {
  if (settings_->value(kSound).toString() == kOff) {
    settings_->setValue(kSound, kOn);
  } else {
    settings_->setValue(kSound, kOff);
  }
  View::Instance().SetSound(settings_->value(kSound).toString());
}

void Model::SetDefaultSettings() {
  settings_->setValue(kComplexity, kEasy);
  settings_->setValue(kSound, kOn);
  settings_->setValue(kExitShortcut, kDefaultExitShortcut);
  View::Instance().SetComplexityButton(
      settings_->value(kComplexity).toString()
  );
  View::Instance().SetExitShortcut(
      "Exit shortcut: " + settings_->value(kExitShortcut).toString()
  );
  View::Instance().SetSound(settings_->value(kSound).toString());
  Controller::Instance().DisconnectShortcutSignals(
      View::Instance(),
      Model::Instance());
  exit_shortcut_ = new QShortcut(
      QKeySequence(kDefaultExitShortcut),
      &View::Instance()
  );
  Controller::Instance().ConnectShortcutSignals(
      View::Instance(),
      Model::Instance());
}
