#include "level.h"
#include "view.h"
#include "model.h"

Level::Level(const QString& level_name,
             const QString& difficulty,
             int day_index) {
  path_ = ":/levels/" + level_name + "/";
  View::Instance().LoadBackgroundFrames(path_ + "background");
  auto stats = FileLoader::GetFile
      <QJsonDocument>(path_ + "stats.json");
  auto stats_dependent = stats[difficulty];
  errors_max_ = stats_dependent["errors_max"].toInt();
  duration_seconds_ = stats_dependent["duration_seconds"].toInt();
  guests_max_ = stats_dependent["guests_count"].toInt();
  level_intro_ = stats["intro"].toString();
  queue_threshold_millis_ = stats["queue_threshold_millis"].toInt();
  for (auto trait : stats["traits"].toArray()) {
    traits_.push_back(trait.toString());
  }
  for (auto dude : stats["dudes"].toArray()) {
    dudes_.push_back(dude.toString());
  }
  int index = 0;
  auto rewards = stats_dependent["rewards"].toArray();
  auto penalties = stats_dependent["penalties"].toArray();
  for (auto entry : stats["days_data"].toArray()) {
    days_.emplace_back();
    auto& day = days_.back();
    day.intro_ = entry.toObject()["intro"].toString();
    for (auto name: entry.toObject()["allow"].toArray()) {
      day.allowed_dudes_.insert(name.toString());
    }
    day.reward_ = rewards[index].toInt();
    day.penalty_ = penalties[index].toInt();
    ++index;
  }
  InitState(day_index);
  GenerateGuests();
}

void Level::InitState(int day) {
  state_.current_day_ = day;
  state_.errors_count_ = 0;
  state_.guests_left_ = guests_max_;
  state_.seconds_left_ = duration_seconds_;
  state_.guests_left_ = guests_max_;
  state_.was_added_time_ = false;
  state_.next_guest_index_ = 0;
}

void Level::GenerateGuests() {
  state_.guests_left_ = guests_max_;
  guests_.clear();
  for (int i = 0; i < guests_max_; ++i) {
    guests_.push_back(std::make_unique<Guest>());
    auto& guest = guests_.back();
    auto name = Random::VectorChoose(dudes_);
    for (const auto& trait : traits_) {
      if (Random::RandomBool()) {
        name += "_" + trait;
      }
    }
    guest->good_ = GetCurrentDay().allowed_dudes_.contains(name);
    auto pixmap = FileLoader::GetFile
        <QPixmap>(path_ + "dudes/" + name + ".png");
    View::Instance().AddGuestSprite(guest.get(), pixmap);
    guest->sprite_->hide();
  }
}

void Level::SetDay(int day) {
  guests_.clear();
  state_.current_day_ = day;
  used_ignore_fist_mistake = false;
  InitState(day);
  GenerateGuests();
}

bool Level::NextDay() {
  if (++state_.current_day_ == days_.size()) {
    return false;
  }
  SetDay(state_.current_day_);
  return true;
}

void Level::AddTime(int time) {
  if (!state_.was_added_time_) {
    state_.seconds_left_ += time;
    state_.was_added_time_ = true;
  }
}

Guest* Level::GetNextGuest() {
  if (state_.next_guest_index_ < guests_max_) {
    return guests_[state_.next_guest_index_].get();
  }
  return nullptr;
}

void Level::ShiftNextGuest() {
  ++state_.next_guest_index_;
}

void Level::IncrementMistakes() {
  ++state_.errors_count_;
  if (Model::Instance().HasItem(kIgnoreFirstMistake) &&
      !used_ignore_fist_mistake) {
    --state_.errors_count_;
    used_ignore_fist_mistake = true;
  }
}
