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
  ConnectCasinoSignals();
  ConnectBlackJackSignals();
  ConnectFruitMachineSignals();
  ConnectChooseGameSignals();
  ConnectShopSignals();
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
  auto continue_button = View::Instance().GetContinueButton();
  if (obj == static_cast<QObject*>(continue_button)) {
    if (event->type() == QEvent::Enter) {
      auto level_name = Model::Instance().LoadSettingsLevel().replace('_', ' ');
      if (!level_name.isEmpty()) {
        auto day_number = Model::Instance().LoadSettingsDay();
        continue_button->setText("Level: " + level_name +
            "\nDay " + day_number);
      } else {
        continue_button->setText("New Game");
      }
    } else if (event->type() == QEvent::Leave) {
      continue_button->setText("Continue");
    }
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
  connect(View::Instance().GetPauseGameButton(),
          &QPushButton::released,
          this,
          [&] {
      View::Instance().GamePauseStart();
      Model::Instance().GetDayTimer()->stop();
      View::Instance().GetTimer()->stop();
      View::Instance().DisableGameButtons();
      is_game_pause = true;
  });
  connect(View::Instance().GetGameExitButton(),
          &QPushButton::released,
          this,
          [&] {
      View::Instance().GamePauseFinish();
      View::Instance().ShowMainMenu();
      View::Instance().EnableGameButtons();
      is_game_pause = false;
  });
  connect(View::Instance().GetGameContinueButton(),
          &QPushButton::released,
          this,
          [&] {
      View::Instance().GamePauseFinish();
      Model::Instance().GetDayTimer()->start();
      View::Instance().GetTimer()->start();
      View::Instance().EnableGameButtons();
      is_game_pause = false;
  });
  connect(View::Instance().GetToMainMenuButton(),
          &QPushButton::released,
          this,
          [&] {
      View::Instance().GameOverHide();
      View::Instance().ShowMainMenu();
  });
  connect(View::Instance().GetToCasinoButton(),
          &QPushButton::released,
          this,
          [&]{
            View::Instance().GameOverHide();
            View::Instance().ShowCasino();
  });
  connect(View::Instance().GetToShopButton(),
          &QPushButton::released,
          this,
          [&]{
            View::Instance().GameOverHide();
            View::Instance().ShowShop();
  });
}

void Controller::ConnectMainMenuSignals() {
  connect(View::Instance().GetOpenSettingsButton(), &QPushButton::released,
          &View::Instance(), &View::ShowSettings);
  connect(View::Instance().GetStartGameButton(), &QPushButton::released,
         &View::Instance(), &View::ShowChooseGame);
  connect(View::Instance().GetQuitButton(), &QPushButton::released,
         this, &Controller::Quit);
}

void Controller::ConnectCasinoSignals() {
  QPushButton::connect(View::Instance().GetBlackJackButton(),
                       &QPushButton::pressed,
                       &View::Instance(),
                       &View::ShowBlackJack);
  QPushButton::connect(View::Instance().GetExitCasino(),
                       &QPushButton::released,
                       &View::Instance(),
                       &View::ShowChooseGame);
}

void Controller::ConnectBlackJackSignals() {
  QPushButton::connect(View::Instance().GetExitBlackJackButton(),
                       &QPushButton::pressed,
                       &View::Instance(),
                       &View::ShowCasino);
  QPushButton::connect(View::Instance().GetMakeBidButton(),
                       &QPushButton::pressed,
                       &Model::Instance(),
                       &Model::StartNewGameBlackJack);
  QPushButton::connect(View::Instance().GetHitMeButton(),
                       &QPushButton::pressed,
                       this,
                       []{
    BlackJack::Instance().HitPlayer();
  });

  QPushButton::connect(View::Instance().GetStandButton(),
                       &QPushButton::pressed,
                       this,
                       []{
    BlackJack::Instance().HitCroupier();
  });
  QPushButton::connect(View::Instance().GetFruitMachineButton(),
                       &QPushButton::pressed,
                       &View::Instance(),
                       &View::ShowFruitMachine);
}

void Controller::ConnectShortcutSignals() {
  QShortcut::connect(Model::Instance().GetExitShortcut(),
                     &QShortcut::activated,
                     this,
                     &Controller::Quit);
}

void Controller::ConnectFruitMachineSignals() {
  QPushButton::connect(View::Instance().GetExitFruitMachine(),
                       &QPushButton::pressed,
                       &View::Instance(),
                       &View::ShowCasino);
  QPushButton::connect(View::Instance().GetMakeBidFruitMachine(),
                       &QPushButton::pressed,
                       &Model::Instance(),
                       &Model::StartFruitMachineGame);
}

void Controller::ConnectChooseGameSignals() {
  connect(View::Instance().GetContinueButton(),
          &QPushButton::released,
          &Model::Instance(),
          &Model::ContinueGame);
  connect(View::Instance().GetNewGameButton(),
          &QPushButton::released,
          &Model::Instance(),
          &Model::StartNewGame);
  connect(View::Instance().GetToMenuFromChooseGameButton(),
          &QPushButton::released,
          &View::Instance(),
          &View::ShowMainMenu);
  connect(View::Instance().GetToCasinoFromChooseButton(),
          &QPushButton::released,
          &View::Instance(),
          &View::ShowCasino);
  connect(View::Instance().GetToShopFromChooseButton(),
          &QPushButton::released,
          &View::Instance(),
          &View::ShowShop);

  View::Instance().GetContinueButton()->installEventFilter(this);
}

void Controller::ConnectShopSignals() {
  QPushButton::connect(View::Instance().GetStandTheWorldBuy(),
                       &QPushButton::released,
                       &Model::Instance(),
                        &Model::BuyTimeItem);
  QPushButton::connect(View::Instance().GetVabankBuy(),
                       &QPushButton::released,
                       &Model::Instance(),
                       &Model::BuyAllInItem);
  QPushButton::connect(View::Instance().GetPandemicBuy(),
                       &QPushButton::released,
                       &Model::Instance(),
                       &Model::BuyReduceGuestsItem);
  QPushButton::connect(View::Instance().GetKsivaBuy(),
                       &QPushButton::released,
                       &Model::Instance(),
                       &Model::BuyAddIgnoreFirstMistakeItem);
  QPushButton::connect(View::Instance().GetExitShop(),
                       &QPushButton::released,
                       &View::Instance(),
                       &View::ShowChooseGame);
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
      if (!is_game_pause) {
        model.Reject();
      }
      break;
    }
    case Qt::Key_A: {
      if (!is_game_pause) {
        model.Permit();
      }
      break;
    }
    case Qt::Key_Space: {
      if (!is_game_pause) {
        View::Instance().HideIntro();
        Model::Instance().GetDayTimer()->start();
        View::Instance().GetTimer()->start();
        View::Instance().EnableGameButtons();
      }
    }
  }
}

void Controller::Quit() {
  if (!is_editing_keybinding_) {
    View::Instance().close();
  }
}

void Controller::StartAnimation() {
  is_playing_animation_ = true;
}

void Controller::StopAnimation() {
  is_playing_animation_ = false;
}
