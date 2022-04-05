#include "model.h"
#include "mvc.h"

Model::Model(Controller* controller)
    : controller_(controller) {
  for (int i = 0; i < kQueueLength; ++i) {
    queue_.push_back(new Visitor);
  }
  current_ = new Visitor;
}

void Model::Paint() {
  view_->RenderActiveVisitor(current_);
  view_->RenderQueue(queue_);
}

void Model::Permit() {
  errors_ += !current_->Good();
  ShiftQueue();
  Paint();
  UpdateErrors();
}

void Model::Reject() {
  errors_ += current_->Good();
  ShiftQueue();
  Paint();
  UpdateErrors();
}

void Model::ShiftQueue() {
  delete current_;
  current_ = queue_.front();
  queue_.pop_front();
  queue_.push_back(new Visitor);
}

void Model::UpdateErrors() {
  view_->SetErrorCount(errors_);
}
