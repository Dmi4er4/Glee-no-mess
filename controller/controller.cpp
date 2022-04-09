#include "controller.h"
#include "model.h"
#include "view.h"

Controller::Controller() {
  ConnectSignals();
  auto& view = View::Instance();
  view.setWindowTitle("Glee, no mess!");
  view.show();
}

void Controller::ConnectSignals() {
  auto& view = View::Instance();
  auto& model = Model::Instance();
  QAbstractButton::connect(
      view.GetPermitButton(), &QPushButton::pressed,
      &model, &Model::Permit);
  QAbstractButton::connect(
      view.GetRejectButton(), &QPushButton::pressed,
      &model, &Model::Reject);
}

void Controller::keyPressEvent(QKeyEvent* event) {
  auto& model = Model::Instance();
  switch (event->key()) {
  case Qt::Key::Key_D: {
    model.Reject();
    break;
  }
  case Qt::Key::Key_A: {
    model.Permit();
    break;
  }
  }
}

Controller& Controller::Instance() {
  static Controller instance;
  return instance;
}
