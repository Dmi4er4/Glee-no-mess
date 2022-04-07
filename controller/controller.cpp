#include "mvc.h"

Controller::Controller() {
  ConnectSignals();
  Model::Paint();
  Model::UpdateErrors();
  View::Instance().setWindowTitle("Glee, no mess!");
  View::Instance().show();
}

void Controller::ConnectSignals() {
  QAbstractButton::connect(
      View::GetPermitButton(), &QPushButton::pressed,
      &Model::Instance(), &Model::Permit);
  QAbstractButton::connect(
      View::GetRejectButton(), &QPushButton::pressed,
      &Model::Instance(), &Model::Reject);
}

void Controller::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
  case Qt::Key::Key_D: {
    Model::Reject();
    break;
  }
  case Qt::Key::Key_A: {
    Model::Permit();
    break;
  }
  }
}

Controller& Controller::Instance() {
  static Controller instance;
  return instance;
}
