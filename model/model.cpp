#include "model.h"

#include <utility>

#include "file_loader.h"
#include "view.h"
#include "controller.h"

Model::Model() {
  settings_ = new QSettings("GameMasters", "Glee-no-mess");
  day_timer_ = std::make_unique<QTimer>();
  connect(day_timer_.get(), &QTimer::timeout,
          this, &Model::DayTimerTick);
  ReadLevels();
  InitSettings();
}

void Model::Permit() {
  auto current_dude = queue_.front();
  if (!current_dude->IsGood()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
}

void Model::Reject() {
  auto current_dude = queue_.front();
  if (!current_dude->IsGood()) {
    IncreaseErrorsCount();
  }
  ShiftQueue();
}

void Model::ShiftQueue() {
  queue_.front()->GetSprite()->hide();
  queue_.pop_front();
  if (queue_.empty() && level_->GetNextGuest() == nullptr) {
    return DayPassed();
  }
  Controller::Instance().StartAnimation();
  level_->DecrementGuestsRemaining();
}

void Model::IncreaseErrorsCount() {
  level_->IncrementMistakes();
  if (level_->DayFailed()) {
    DayFailed();
  }
}

Model& Model::Instance() {
  static Model instance;
  return instance;
}

void Model::AddTime(size_t time) {
  level_->AddTime(time);
}

bool Model::HasItem(const QString& name) {
  for (const auto& item : all_items_) {
    if (item->GetName() == name) {
      return true;
    }
  }
  return false;
}

void Model::AddTimeItem() {
  if (!HasItem(kAddTime)) {
    all_items_.emplace_back(new TimeItem);
  }
}

void Model::AddIgnoreFirstMistakeItem() {
  if (!HasItem(kIgnoreFirstMistake)) {
    all_items_.emplace_back(new IgnoreFirstMistakeItem);
  }
}

void Model::InitSettings() {
  auto file = FileLoader::GetFile<QJsonDocument>(kDefaultSettings);
  for (const auto& property_name : {kMoney,
                                   kSound,
                                   kDifficulty,
                                   kExitShortcut,
                                   kIgnoreFirstMistake,
                                   kAddTime,
                                   kAllin,
                                   kReduceGuest}) {
    InitSetting(file, property_name);
  }
  View::Instance().SetDifficulty(
      settings_->value(kDifficulty).toString());
  View::Instance().SetExitShortcut(
      settings_->value(kExitShortcut).toString());
  View::Instance().SetSound(settings_->value(kSound).toString());
  exit_shortcut_ = new QShortcut(
      QKeySequence(settings_->value(kExitShortcut).toString()),
      &View::Instance());
  View::Instance().SetPlayerMoney(settings_->value(kMoney).toInt());
  View::Instance().SetPlayerMaxBid(settings_->value(kMoney).toInt());

  if (settings_->value(kIgnoreFirstMistake).toBool()) {
    View::Instance().HideKsive();
    AddIgnoreFirstMistakeItem();
  }
  if (settings_->value(kAddTime).toBool()) {
    View::Instance().HideStandTheWorld();
    AddTimeItem();
  }
  if (settings_->value(kAllin).toBool()) {
    View::Instance().HideVabank();
    AddAllInItem();
  }
  if (settings_->value(kReduceGuest).toBool()) {
    View::Instance().HidePandemic();
    AddReduceGuestsItem();
  }

  if (IsSoundOn()) {
    View::Instance().ShowMainMenu();
  }
}

void Model::InitSetting(const QJsonDocument& json, const QString& property) {
  if (!settings_->contains(property)) {
    settings_->setValue(property, json[property]);
  }
}

void Model::ChangeDifficulty() {
  if (settings_->value(kDifficulty) == kEasy) {
    settings_->setValue(kDifficulty, kMedium);
  } else if (settings_->value(kDifficulty) == kMedium) {
    settings_->setValue(kDifficulty, kHard);
  } else {
    settings_->setValue(kDifficulty, kEasy);
  }
  View::Instance().SetDifficulty(
      settings_->value(kDifficulty).toString());
}

void Model::SetExitShortcut(const QString& keys) {
  settings_->setValue(kExitShortcut, keys);
  exit_shortcut_->setKey(keys);
  View::Instance().SetExitShortcut(keys);
}

void Model::ToggleSound() {
  if (settings_->value(kSound) == kOff) {
    settings_->setValue(kSound, kOn);
    View::Instance().SetSound(settings_->value(kSound).toString());
    View::Instance().ShowSettings();
  } else {
    settings_->setValue(kSound, kOff);
    View::Instance().SetSound(settings_->value(kSound).toString());
    View::Instance().StopAllSounds();
  }
}

void Model::ResetDefaults() {
  auto default_settings =
      FileLoader::GetFile<QJsonDocument>(kDefaultSettings);
  const QString default_sound = default_settings[kSound].toString();
  const QString default_difficulty = default_settings[kDifficulty].toString();
  const QString default_exit_shortcut =
      default_settings[kExitShortcut].toString();

  settings_->setValue(kDifficulty, default_difficulty);
  settings_->setValue(kExitShortcut, default_exit_shortcut);
  settings_->setValue(kSound, default_sound);

  View::Instance().SetDifficulty(default_difficulty);
  View::Instance().SetExitShortcut(default_exit_shortcut);
  View::Instance().SetSound(default_sound);
  View::Instance().ShowSettings();
  exit_shortcut_->setKey(QKeySequence(default_exit_shortcut));
}

void Model::StartNewGameBlackJack() {
  auto bid = View::Instance().GetBid();
  if (bid == 0) {
    return;
  }
  View::Instance().ShowBlackJackGame();
  View::Instance().ShowSatus("");
  UpdateMoney(-bid);
  BlackJack::Instance().StartNewGame(bid);
}

void Model::UpdateMoney(money_t delta) {
  money_t money = settings_->value(kMoney).toInt();
  money += delta;
  settings_->setValue(kMoney, QString::number(money));
  View::Instance().SetPlayerMoney(money);
  View::Instance().SetPlayerMaxBid(money);
}

void Model::StartFruitMachineGame() {
  static constexpr int delay[] = {2000, 3000, 4000};

  View::Instance().GetMakeBidFruitMachine()->setEnabled(false);
  auto bid = View::Instance().GetFruitMachineBid();
  if (bid == 0) {
    return;
  }
  UpdateMoney(-bid);
  for (int& new_slot_pic : new_slot_pics_) {
    new_slot_pic = Random::RandomInt(0, 8);
  }

  static const QString path = ":casino/machine_";

  auto ChangePicture = [](int slot, int picture) {
    View::Instance().
        SetFruitMachineSlot(slot,
                            FileLoader::GetFile<QPixmap>(
                                path + QString::number(picture) + ".png"));
  };
  auto ChangeBorder = [](int slot, bool is_spinning) {
    View::Instance().SetFruitMachineSlotBorder(slot, is_spinning);
  };

  for (int i = 0; i < kSlotsCount; i++) {
    ChangeBorder(i, true);
  }
  spinning_ = kSlotsCount;

  slot_update_timer_ = new QTimer;
  connect(slot_update_timer_, &QTimer::timeout, this, [ChangePicture, this] {
    for (int i = kSlotsCount - 1; i >= kSlotsCount - spinning_; --i) {
      ChangePicture(i, Random::RandomInt(0, 8));
    }
  });
  slot_update_timer_->start(100);

  for (int i = 0; i < kSlotsCount - 1; ++i) {
    QTimer::singleShot(std::accumulate(delay, delay + i + 1, 0),
                       [ChangePicture, ChangeBorder, this, i] {
                         --spinning_;
                         ChangeBorder(i, false);
                         ChangePicture(i, new_slot_pics_[i]);
                       });
  }

  QTimer::singleShot(std::accumulate(delay, delay + kSlotsCount, 0),
                     [ChangePicture, ChangeBorder, bid, this] {
                       ChangeBorder(kSlotsCount - 1, false);
                       ChangePicture(kSlotsCount - 1,
                                     new_slot_pics_[kSlotsCount - 1]);
                       --spinning_;
                       delete slot_update_timer_;

                       bool all_equal = true;
                       bool any_equal = false;
                       std::sort(new_slot_pics_, new_slot_pics_ + kSlotsCount);
                       for (int i = 1; i < kSlotsCount; ++i) {
                         if (new_slot_pics_[i] != new_slot_pics_[i - 1]) {
                           all_equal = false;
                         } else {
                           any_equal = true;
                         }
                       }

                       if (all_equal) {
                         UpdateMoney(bid);
                         if (new_slot_pics_[0] == kBigWinIndex) {
                           UpdateMoney(kBigWinRewardCoef * bid);
                         } else {
                           UpdateMoney(kAllEqualRewardCoef * bid);
                         }
                       } else if (any_equal) {
                         UpdateMoney(bid);
                         UpdateMoney(kAnyEqualRewardCoef * bid);
                       }

                       View::Instance().
                           GetMakeBidFruitMachine()->setEnabled(true);
                     });
}

void Model::AddAllInItem() {
  if (!HasItem(kAllin)) {
    all_items_.emplace_back(new Item(kAllin));
  }
}

void Model::AddReduceGuestsItem() {
  if (!HasItem(kReduceGuest)) {
    all_items_.emplace_back(new Item(kReduceGuest));
  }
}

void Model::BuyAddIgnoreFirstMistakeItem() {
  money_t money = settings_->value(kMoney).toInt();
  if (money >= kCost) {
    UpdateMoney(-kCost);
    AddIgnoreFirstMistakeItem();
    View::Instance().HideKsive();
    settings_->setValue(kIgnoreFirstMistake, "1");
  }
}

void Model::BuyTimeItem() {
  money_t money = settings_->value(kMoney).toInt();
  if (money >= kCost) {
    UpdateMoney(-kCost);
    AddTimeItem();
    View::Instance().HideStandTheWorld();
    settings_->setValue(kAddTime, "1");
  }
}

void Model::BuyAllInItem() {
  money_t money = settings_->value(kMoney).toInt();
  if (money >= kCost) {
    UpdateMoney(-kCost);
    AddAllInItem();
    View::Instance().HideVabank();
    settings_->setValue(kAllin, "1");
  }
}

void Model::BuyReduceGuestsItem() {
  money_t money = settings_->value(kMoney).toInt();
  if (money >= kCost) {
    UpdateMoney(-kCost);
    AddReduceGuestsItem();
    View::Instance().HidePandemic();
    settings_->setValue(kReduceGuest, "1");
  }
}

void Model::ReadLevels() {
  level_names_.clear();
  auto array = FileLoader::GetFile<QJsonDocument>(":levels/levels_list.json");
  for (auto array_item : array.array()) {
    level_names_.push_back(array_item.toString());
  }
  level_ = std::make_unique<Level>(level_names_[current_level_index_ = 0],
                                   GetDifficulty());
}

QString Model::LoadSettingsLevel() {
  auto json = CurrentSave();
  if (json.isEmpty()) {
    return "";
  }
  return json["level"].toString();
}

QString Model::LoadSettingsDay() {
  auto json = CurrentSave();
  return QString::number(json["day"].toInt() + 1);
}

QJsonObject Model::CurrentSave() {
  auto key_of_save = "save_" + GetDifficulty();
  return settings_->value(key_of_save).toJsonObject();
}

void Model::DayPassed() {
  UpdateMoney(+level_->GetCurrentDay().reward_);
  if (!level_->NextDay()) {
    ++current_level_index_;
  }
  SaveGame(level_names_[current_level_index_], level_->GetDayIndex());
  View::Instance().GameOverShow();
  View::Instance().WinGame();
}

void Model::DayFailed() {
  UpdateMoney(-level_->GetCurrentDay().penalty_);
  View::Instance().GameOverShow();
  View::Instance().LooseGame();
}

void Model::StartDay() {
  queue_.clear();
  Controller::Instance().StartAnimation();
  View::Instance().ShowGame();
  SaveGame(level_names_[current_level_index_], level_->GetDayIndex());
  day_timer_->start(1000);
  View::Instance().SetIntro(level_->GetCurrentDay().intro_);
  View::Instance().ShowIntro();
  day_timer_->stop();
  View::Instance().GetTimer()->stop();
  View::Instance().DisableGameButtons();
}

void Model::StartNewGame() {
  current_level_index_ = 0;
  level_ = std::make_unique<Level>(level_names_.front(), GetDifficulty());
  StartDay();
}

void Model::ContinueGame() {
  if (!TryLoadSave()) {
    StartNewGame();
  }
}

bool Model::TryLoadSave() {
  auto json = CurrentSave();
  if (json.isEmpty()) {
    return false;
  }
  auto level_name = json["level"].toString();
  current_level_index_ = std::find(level_names_.begin(), level_names_.end(),
                                   level_name) - level_names_.begin();
  level_ = std::make_unique<Level>(level_name, GetDifficulty());
  level_->InitState(json["day"].toInt());
  level_->GenerateGuests();
  StartDay();
  return true;
}

void Model::SaveGame(const QString& level_name, int day_index) {
  auto key_save = "save_" + GetDifficulty();
  QJsonObject json;
  json["level"] = level_name;
  json["day"] = day_index;
  settings_->setValue(key_save, json);
}

void Model::DoAnimation(int millis) {
  if (!Controller::Instance().IsPlayingAnimation()) {
    return;
  }
  bool finished = false;
  for (int i = 0; i < queue_.size(); ++i) {
    finished = !queue_[i]->DoAnimation(millis, i);
  }
  if (finished) {
    Controller::Instance().StopAnimation();
    return;
  }
  auto next = level_->GetNextGuest();
  if (next != nullptr && (queue_.empty() ||
      queue_.back()->GetAnimation()->GetTimePassed() >=
      level_->GetQueueThreshold())) {
    queue_.push_back(next);
    auto pushed = queue_.back();
    level_->ShiftNextGuest();
    pushed->SetAnimation(level_->GetPath() + "animation_queue.json");
    pushed->GetSprite()->show();
  }
}

void Model::DayTimerTick() {
  if (!level_->DecrementTimeLeft()) {
    day_timer_->stop();
    DayFailed();
  }
}
