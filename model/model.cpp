#include "model.h"

#include "controller.h"
#include "view.h"

#include <utility>

int Model::errors_ = 0;

Model::Model() {
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.emplace_back(new Guest);
  }
  current_guest_ = std::make_unique<Guest>();
}

void Model::Paint() {
  View::RenderActiveVisitor(Instance().current_guest_.get());
  View::RenderQueue(Instance().queue_);
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
  auto& m = Instance();
  m.current_guest_ = std::move(m.queue_.front());
  m.queue_.pop_front();
  m.queue_.emplace_back(new Guest);
}

void Model::IncreaseErrorsCount() {
  View::SetErrorCount(++errors_);
}

Model& Model::Instance() {
  static Model instance;
  return instance;
}
