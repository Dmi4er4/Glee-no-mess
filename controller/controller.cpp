#include "mvc.h"
#include "controller.h"

Controller::Controller() {
  has_instance_ = true;
  Model::Instance();
  View::Instance();

  ConnectSignals();
  Model::Instance().Paint();
  Model::Instance().UpdateErrors();
  View::Instance().setWindowTitle("Glee, no mess!");
  View::Instance().show();
}

void Controller::ConnectSignals() {
  QAbstractButton::connect(
      View::Instance().GetPermitButton(), &QPushButton::pressed,
      &Model::Instance(), &Model::Permit);
  QAbstractButton::connect(
      View::Instance().GetRejectButton(), &QPushButton::pressed,
      &Model::Instance(), &Model::Reject);
}

void Controller::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
  case Qt::Key::Key_D: {
    Model::Instance().Reject();
    break;
  }
  case Qt::Key::Key_A: {
    Model::Instance().Permit();
    break;
  }
  }
}

Controller& Controller::Instance() {
  static Controller instance;
  return instance;
}

void Controller::Init() {
  if (!has_instance_) {
    Instance();
  }
}
