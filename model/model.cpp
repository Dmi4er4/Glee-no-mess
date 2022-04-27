#include "model.h"

#include "all_items.h"
#include "controller.h"
#include "view.h"

#include <utility>

size_t Model::errors_count = 0;
size_t Model::time_left = 0;
bool Model::is_first_mistake = true;
bool Model::was_added_time = false;
std::vector<std::shared_ptr<Item>> Model::all_items = {};

Model::Model() {
  auto& view = View::Instance();
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.emplace_back(std::make_unique<Guest>());
    queue_.back()->SetIndex(i);
  }
  current_guest_ = std::make_unique<Guest>();
  current_guest_->SetActive();
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
  errors_count++;
  for (auto item : all_items) {
    item->MistakeTrigger();
  }
  is_first_mistake = false;
  // TODO(Adamenko-Vladislav)
}

void Model::StartNewLevel() {
  errors_count = 0;
  is_first_mistake = true;
  // time_lest =
  was_added_time = false;
  // TODO(Adamenko-Vladislav)
}

void Model::AddTime(size_t time) {
  if (!was_added_time) {
    time_left += time;
    was_added_time = true;
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
