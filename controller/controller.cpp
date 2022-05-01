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
  auto& model = Model::Instance();
  auto& view = View::Instance();

  if (view.IsGame()) {
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
  } else if (view.IsSettings()) {
    if (view.IsCursorOnExitShortcut()) {
      switch (event->key()) {
        case Qt::Key_Alt:
        case Qt::Key_Shift:
        case Qt::Key_Control: {
          break;
        }
        default: {
          QString answer = "Exit shortcut: ";
          QString keys;
          if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            answer += "Ctrl+";
            keys += "Ctrl+";
          }
          if (QApplication::keyboardModifiers() & Qt::AltModifier) {
            answer += "Alt+";
            keys += "Alt+";
          }
          if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
            answer += "Shift+";
            keys += "Shift+";
          }
          answer +=
              QKeySequence(event->key()).toString(QKeySequence::NativeText);
          keys +=
              QKeySequence(event->key()).toString(QKeySequence::NativeText);
          view.SetExitShortcut(answer);
          model.SetExitShortcut(keys);
        }
      }
    }
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
