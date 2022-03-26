#include "mvc.h"
#include "controller.h"

#include <QPushButton>

Controller::Controller()
    : view_(new View(this)),
      model_(new Model(this)) {
  view_->model_ = model_.get();
  model_->view_ = view_.get();
  ConnectSignals();
  model_->Paint();
  view_->show();
}

void Controller::ConnectSignals() {
  QAbstractButton::connect(view_->ok_, &QPushButton::pressed,
                           model_.get(), &Model::Permit);
  QAbstractButton::connect(view_->not_ok_, &QPushButton::pressed,
                           model_.get(), &Model::Reject);
}
