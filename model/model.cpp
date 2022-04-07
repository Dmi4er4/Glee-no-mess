#include "mvc.h"

Model::Model() {
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.push_back(new Guest);
  }
  current_ = new Guest;
}

void Model::Paint() {
  View::RenderActiveVisitor(Instance().current_);
  View::RenderQueue(Instance().queue_);
}

void Model::Permit() {
  auto& m = Instance();
  m.errors_ += !m.current_->IsMale();
  ShiftQueue();
  Paint();
  UpdateErrors();
}

void Model::Reject() {
  auto& m = Instance();
  m.errors_ += m.current_->IsMale();
  ShiftQueue();
  Paint();
  UpdateErrors();
}

void Model::ShiftQueue() {
  auto& m = Instance();
  delete m.current_;
  m.current_ = m.queue_.front();
  m.queue_.pop_front();
  m.queue_.push_back(new Guest);
}

void Model::UpdateErrors() {
  View::SetErrorCount(Instance().errors_);
}

Model& Model::Instance() {
  static Model instance;
  return instance;
}
