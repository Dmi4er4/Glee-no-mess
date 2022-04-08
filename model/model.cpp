#include "model.h"

#include "controller.h"
#include "view.h"

#include <utility>

Model::Model() {
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.emplace_back(new Guest);
  }
  current_guest_ = std::make_unique<Guest>();
}

void Model::Paint() {
  auto& view = View::Instance();
  view.RenderActiveGuest(current_guest_.get());
  view.RenderQueue(queue_);
}

void Model::Permit() {
  if (!Instance().current_guest_->IsMale()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
  Paint();
}

void Model::Reject() {
  if (Instance().current_guest_->IsMale()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
  Paint();
}

void Model::ShiftQueue() {
  current_guest_ = std::move(queue_.front());
  queue_.pop_front();
  queue_.emplace_back(new Guest);
}

void Model::IncreaseErrorsCount() {
  View::Instance().SetErrorCount(++errors_);
}

Model& Model::Instance() {
  static Model instance;
  return instance;
}
