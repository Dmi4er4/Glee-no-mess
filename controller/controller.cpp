#include "controller.h"

#include<QShortcut>

#include "model.h"
#include "view.h"

Controller::Controller() {
  ConnectSignals();
  auto& view = View::Instance();
  view.setWindowTitle("Glee, no mess!");
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
    KeyPressInSettings(event);
  }
}

Controller& Controller::Instance() {
  static Controller instance;
  return instance;
}

void Controller::ConnectSettingsMenuSignals() {
  connect(View::Instance().GetDifficultyButton(),
                       &QPushButton::released,
                       &Model::Instance(),
                       &Model::ChangeDifficulty);
  connect(View::Instance().GetBackToMenuButton(),
                       &QPushButton::released,
                       &View::Instance(),
                       &View::ShowMainMenu);
  connect(View::Instance().GetSoundButton(),
                       &QPushButton::released,
                       &Model::Instance(),
                       &Model::ToggleSound);
  connect(View::Instance().GetDefaultSettingsButton(),
                       &QPushButton::released,
                       &Model::Instance(),
                       &Model::ResetDefaults);
  connect(View::Instance().GetExitShortcutButton(),
                       &QPushButton::released,
                       this,
                       [&] {
      View::Instance().RequestKeyComboEnter();
      is_editing_keybinding_ = true;
      new_hotkey_ = [capture0 = &Model::Instance()](auto && PH1)
          { capture0->SetExitShortcut(std::forward<decltype(PH1)>(PH1)); };
  });
}

void Controller::ConnectGameSignals() {
  connect(
      View::Instance().GetPermitButton(), &QPushButton::released,
      &Model::Instance(), &Model::Permit);
  connect(
      View::Instance().GetRejectButton(), &QPushButton::released,
      &Model::Instance(), &Model::Reject);
}

void Controller::ConnectMainMenuSignals() {
  connect(View::Instance().GetOpenSettingsButton(),
                       &QPushButton::released,
                       &View::Instance(),
                       &View::ShowSettings);
  connect(View::Instance().GetStartGameButton(),
                       &QPushButton::released,
                       &View::Instance(),
                       &View::ShowGame);
  connect(View::Instance().GetQuitButton(),
                       &QPushButton::released,
                       this,
                       &Controller::Quit);
}

void Controller::ConnectShortcutSignals() {
  QShortcut::connect(Model::Instance().GetExitShortcut(),
                     &QShortcut::activated,
                     this,
                     &Controller::Quit);
}

void Controller::KeyPressInSettings(QKeyEvent* event) {
  if (is_editing_keybinding_) {
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
        new_hotkey_(keys);
        is_editing_keybinding_ = false;
        View::Instance().HideShortcutRequestOverlay();
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

void Controller::Quit() {
  if (!is_editing_keybinding_) {
    View::Instance().close();
  }
}
