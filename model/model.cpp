#include "model.h"

#include "controller.h"
#include "view.h"

#include <utility>

size_t Model::errors_count_ = 0;
size_t Model::time_left_ = 0;
bool Model::is_first_mistake_ = true;
bool Model::was_added_time_ = false;
std::vector<Items*> Model::all_items_ = {};

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

void Model::ForgiveFirstMistake() {
  if (is_first_mistake_) {
    errors_count_--;
  }
}

void Model::UpdateMistake() {
  errors_count_++;
  for (auto item: all_items_) {
    item->MistakeTrigger();
  }
  is_first_mistake_ = false;
  // TODO
}

void Model::AddIgnoreFirstMistakeItem() {
  for (auto item: all_items_) {
    if (item->GetName() == "ignore_first_mistake") {
      return;
    }
  }
  all_items_.push_back(new IgnoreFirstMistakeItem);
}

void Model::StartNewLevel() {
  errors_count_ = 0;
  is_first_mistake_ = true;
  // time_lest =
  was_added_time_ = false;
  // TODO
}

void Model::AddTime(size_t time) {
  if (!was_added_time_) {
    time_left_ += time;
    was_added_time_ = true;
  }
}

void Model::UpdateTimeLeft() {
  for (auto item: all_items_) {
    item->TimeTrigger();
  }
}

void Model::AddTimeItem() {
  for (auto item: all_items_) {
    if (item->GetName() == "add_time") {
      return;
    }
  }
  all_items_.push_back(new TimeItem);
}
