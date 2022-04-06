#include "model.h"
#include "../mvc.h"

Model::Model(Controller* controller)
    : controller_(controller) {
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.push_back(new Guest);
  }
  current_ = new Guest;
}

void Model::Paint() {
  view_->RenderActiveVisitor(current_);
  view_->RenderQueue(queue_);
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
  view_->SetErrorCount(errors_);
}
