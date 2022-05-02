#include "controller.h"
#include "model.h"
#include "view.h"

#include<QShortcut>

Controller::Controller() {
  ConnectSignals();
  auto& view = View::Instance();
  view.setWindowTitle("Glee, no mess!");
  view.show();
}

void Controller::ConnectSignals() {
  ConnectGameSignals();
  ConnectSettingsMenuSignals();
  ConnectMainMenuSignals();
  ConnectShortcutSignals();
}

void Controller::keyPressEvent(QKeyEvent* event) {
  auto& view = View::Instance();
  if (view.IsGame()) {
    KeyPressInGame(event);
  } else if (view.IsSettings()) {
    KeyPressInSetting(event);
  }
}

Controller& Controller::Instance() {
  static Controller instance;
  return instance;
}

void Controller::ConnectSettingsMenuSignals() {
  QPushButton::connect(View::Instance().GetComplexityButton(),
                       &QPushButton::pressed,
                       &Model::Instance(),
                       &Model::ChangeComplexity);
  QPushButton::connect(View::Instance().GetExitSettingsButton(),
                       &QPushButton::pressed,
                       &View::Instance(),
                       &View::ShowMainMenu);
  QPushButton::connect(View::Instance().GetSoundButton(),
                       &QPushButton::pressed,
                       &Model::Instance(),
                       &Model::ChangeSoundStatus);
  QPushButton::connect(View::Instance().GetDefaultSettingsButton(),
                       &QPushButton::pressed,
                       &Model::Instance(),
                       &Model::SetDefaultSettings);
}

void Controller::ConnectGameSignals() {
  QAbstractButton::connect(
      View::Instance().GetPermitButton(), &QPushButton::pressed,
      &Model::Instance(), &Model::Permit);
  QAbstractButton::connect(
      View::Instance().GetRejectButton(), &QPushButton::pressed,
      &Model::Instance(), &Model::Reject);
}

void Controller::ConnectMainMenuSignals() {
  QPushButton::connect(View::Instance().GetOpenSettingsButton(),
                       &QPushButton::pressed,
                       &View::Instance(),
                       &View::ShowSettings);
  QPushButton::connect(View::Instance().GetStartGameButton(),
                       &QPushButton::pressed,
                       &View::Instance(),
                       &View::ShowGame);
}

void Controller::ConnectShortcutSignals() {
  QShortcut::connect(Model::Instance().GetExitShortcut(),
                     &QShortcut::activated,
                     &View::Instance(),
                     &View::close);
}

void Controller::KeyPressInSetting(QKeyEvent* event) {
  if (View::Instance().IsCursorOnExitShortcut()) {
    switch (event->key()) {
      case Qt::Key_Alt:
      case Qt::Key_Shift:
      case Qt::Key_Control: {
        break;
      }
      default: {
        QString keys = PressedKey(Qt::ControlModifier, "Ctrl+")
            + PressedKey(Qt::AltModifier, "Alt+")
            + PressedKey(Qt::ShiftModifier, "Shift+")
            + QKeySequence(event->key()).toString();
        View::Instance().SetExitShortcut(kExitShortcutText + keys);
        Model::Instance().SetExitShortcut(keys);
      }
    }
  }
}

void Controller::KeyPressInGame(QKeyEvent* event) {
  auto& model = Model::Instance();
  switch (event->key()) {
    case Qt::Key_D: {
      model.Reject();
      break;
    }
    case Qt::Key_A: {
      model.Permit();
      break;
    }
  }
}

QString Controller::PressedKey(Qt::KeyboardModifier key, const QString& text) {
  if (QApplication::keyboardModifiers() & key) {
    return text;
  }
  return "";
}
