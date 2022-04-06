#include "mvc.h"
#include "model.h"

Model::Model() {
  Controller::Init();
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.push_back(new Guest);
  }
  current_ = new Guest;
}

void Model::Paint() {
  View::Instance().RenderActiveVisitor(current_);
  View::Instance().RenderQueue(queue_);
}

void Model::Permit() {
  errors_ += !current_->IsMale();
  ShiftQueue();
  Paint();
  UpdateErrors();
}

void Model::Reject() {
  errors_ += current_->IsMale();
  ShiftQueue();
  Paint();
  UpdateErrors();
}

void Model::ShiftQueue() {
  delete current_;
  current_ = queue_.front();
  queue_.pop_front();
  queue_.push_back(new Guest);
}

void Model::UpdateErrors() {
  View::Instance().SetErrorCount(errors_);
}

Model& Model::Instance() {
  static Model instance;
  return instance;
}
