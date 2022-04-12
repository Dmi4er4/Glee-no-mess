#include "model.h"

#include "controller.h"
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
