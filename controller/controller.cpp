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
  ConnectChooseGameSignals();
}

void Controller::keyPressEvent(QKeyEvent* event) {
  auto& view = View::Instance();
  if (view.IsGame()) {
    KeyPressInGame(event);
  } else if (view.IsSettings()) {
    KeyPressInSettings(event);
  }
}

bool Controller::eventFilter(QObject *obj, QEvent *event) {
  if (obj == static_cast<QObject*>(View::Instance().GetContinueButton())) {
    if (event->type() == QEvent::Enter) {
      View::Instance().GetContinueButton()->setText(
          "Level: Gachi-club\nDay: " +
          QString::number(Model::Instance().GetSettingsDay()));
    } else if (event->type() == QEvent::Leave) {
      View::Instance().GetContinueButton()->setText("Continue");
    }
    return QObject::eventFilter(obj, event);
  }
  return QObject::eventFilter(obj, event);
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
      new_hotkey_ = [model = &Model::Instance()](const QString& keys)
          { model->SetExitShortcut(keys); };
  });
}

void Controller::ConnectGameSignals() {
  connect(
      View::Instance().GetPermitButton(), &QPushButton::released,
      &Model::Instance(), &Model::Permit);
  connect(
      View::Instance().GetRejectButton(), &QPushButton::released,
      &Model::Instance(), &Model::Reject);
  connect(
      View::Instance().GetToMenuFromGameButton(), &QPushButton::released,
      &Model::Instance(), [] () {
        Model::Instance().DayFailed();
      });
}

void Controller::ConnectMainMenuSignals() {
  connect(View::Instance().GetOpenSettingsButton(),
                       &QPushButton::released,
                       &View::Instance(),
                       &View::ShowSettings);
  connect(View::Instance().GetStartGameButton(),
                       &QPushButton::released,
                       &View::Instance(),
                       &View::ShowChooseGame);
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

void Controller::ConnectChooseGameSignals() {
  connect(View::Instance().GetContinueButton(),
          &QPushButton::released,
          this,
          &Controller::ContinueGame);
  connect(View::Instance().GetNewGameButton(),
          &QPushButton::released,
          this,
          &Controller::StartNewGame);
  connect(View::Instance().GetToMenuFromChooseGameButton(),
          &QPushButton::released,
          &View::Instance(),
          &View::ShowMainMenu);

  View::Instance().GetContinueButton()->installEventFilter(this);
}

void Controller::StartNewGame() {
  Model::Instance().SetSettingsDay(1);
  ContinueGame();
}

void Controller::ContinueGame() {
  Model::Instance().StartNewDay();
  View::Instance().SetGuestsLeft(Model::Instance().GetGuestsLeft());
  View::Instance().SetDay(Model::Instance().GetSettingsDay());
  View::Instance().SetTimeLeft(Model::Instance().GetTimeLeft());
  View::Instance().SetErrorsCount(0);
  View::Instance().ShowGame();
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
