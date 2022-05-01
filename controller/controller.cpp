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
  auto& view = View::Instance();
  auto& model = Model::Instance();
  ConnectGameSignals(view, model);
  ConnectSettingsMenuSignals(view, model);
  ConnectMainMenuSignals(view, model);
  ConnectShortcutSignals(view, model);
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
        case Qt::Key_Control: {
          view.SetExitShortcut("Exit shortcut: Ctrl");
          break;
        }
        case Qt::Key_Shift: {
          view.SetExitShortcut("Exit shortcut: Shift");
          break;
        }
        case Qt::Key_Alt: {
          view.SetExitShortcut("Exit shortcut: Alt");
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

void Controller::ConnectSettingsMenuSignals(View& view, Model& model) {
  QPushButton::connect(view.GetComplexityButton(),
                       &QPushButton::pressed,
                       &model,
                       &Model::ChangeComplexity);
  QPushButton::connect(view.GetExitSettingsButton(),
                       &QPushButton::pressed,
                       &view,
                       &View::ShowMainMenu);
  QPushButton::connect(view.GetSoundButton(),
                       &QPushButton::pressed,
                       &model,
                       &Model::ChangeSoundStatus);
  QPushButton::connect(view.GetDefaultSettingsButton(),
                       &QPushButton::pressed,
                       &model,
                       &Model::SetDefaultSettings);
}

void Controller::ConnectGameSignals(View& view, Model& model) {
  QAbstractButton::connect(
      view.GetPermitButton(), &QPushButton::pressed,
      &model, &Model::Permit);
  QAbstractButton::connect(
      view.GetRejectButton(), &QPushButton::pressed,
      &model, &Model::Reject);
}

void Controller::ConnectMainMenuSignals(View& view, Model& model) {
  QPushButton::connect(view.GetOpenSettingsButton(),
                       &QPushButton::pressed,
                       &view,
                       &View::ShowSettings);
  QPushButton::connect(view.GetStartGameButton(),
                       &QPushButton::pressed,
                       &view,
                       &View::ShowGame);
}

void Controller::ConnectShortcutSignals(View& view, Model& model) {
  QShortcut::connect(model.GetExitShortcut(),
                     &QShortcut::activated,
                     &view,
                     &View::close);
}

void Controller::DisconnectShortcutSignals(View& view, Model& model) {
  QShortcut::disconnect(model.GetExitShortcut(),
                        &QShortcut::activated,
                        &view,
                        &View::close);
}
