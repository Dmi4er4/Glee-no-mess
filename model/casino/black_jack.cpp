#include "black_jack.h"

#include "file_loader.h"
#include "model.h"
#include "random.h"
#include "view.h"

BlackJack& BlackJack::Instance() {
  static BlackJack instance;
  return instance;
}

int32_t BlackJack::GetScore(const std::vector<cards::Cards>& card_set) {
  int32_t score = 0;
  for (auto item : card_set) {
    score += item.weight;
  }
  if (score > kMaxScore) {
    score = 0;
    for (auto item : card_set) {
      if (item.value == cards::Value::kAce) {
        score++;
      } else {
        score += item.weight;
      }
    }
  }
  return score;
}

void BlackJack::StartNewGame(size_t bid) {
  View::Instance().ShowBlackJackGame();
  bid_ = bid;
  pack_of_cards_ = PackOfCards();
  croupier_cards_.clear();
  player_cads_.clear();

  number_player_card_ = 0;
  number_croupier_card_ = 0;

  ShowNextCroupierCard();
  ShowNextCroupierCard();

  ShowNextPlayerCard();
  ShowNextPlayerCard();
  CheckStatus();
}

void BlackJack::ShowNextPlayerCard() {
  player_cads_.push_back(pack_of_cards_.GetRandomCard());
  QString file_name = GetCardFileName(player_cads_.back());
  View::Instance().PutPlayerCard(number_player_card_,
                                   FileLoader::GetFile<QPixmap>(file_name));
  number_player_card_++;
}

void BlackJack::ShowNextCroupierCard() {
  croupier_cards_.push_back(pack_of_cards_.GetRandomCard());
  QString file_name = GetCardFileName(croupier_cards_.back());
  View::Instance().PutCroupierCard(number_croupier_card_,
                                   FileLoader::GetFile<QPixmap>(file_name));
  number_croupier_card_++;
}

QString BlackJack::GetCardFileName(const cards::Cards& card) {
  return
      FileLoader::GetFile<QJsonDocument>(":casino/cards.json")
      [kSuitsNames[static_cast<size_t>(card.suit)]]
      [kValueNames[static_cast<size_t>(card.value)]].toString();
}

void BlackJack::Draw() {
  Model::Instance().UpdateMoney(bid_);
  View::Instance().ShowSatus("Draw");
}

void BlackJack::Loose() {
  View::Instance().ShowSatus("You loose!");
}

void BlackJack::Win() {
  Model::Instance().UpdateMoney((bid_ * 3) >> 1);
  View::Instance().ShowSatus("You win!");
}

void BlackJack::CheckStatus() {
  auto player_score = GetScore(player_cads_);
  auto croupier_score = GetScore(croupier_cards_);
  if (player_score == kMaxScore) {
    if (croupier_score == kMaxScore) {
      Draw();
      return;
    }
    Win();
    return;
  } else if (croupier_score == kMaxScore) {
    Loose();
    return;
  }
  if (player_score > kMaxScore) {
    Loose();
    return;
  }
  if (croupier_score > kMaxScore) {
    Win();
  }
}

void BlackJack::HitPlayer() {
  ShowNextPlayerCard();
  auto score = GetScore(player_cads_);
  if (score > kMaxScore) {
    Loose();
  } else if (score == kMaxScore) {
    emit View::Instance().GetStandButton()->pressed();
  }
}

void BlackJack::HitCroupier() {
  while (GetScore(croupier_cards_) < 17) {
    ShowNextCroupierCard();
  }
  CheckStatus();
  auto player_score = GetScore(player_cads_);
  auto croupier_score = GetScore(croupier_cards_);
  if (croupier_score <= kMaxScore) {
    if (croupier_score == player_score) {
      Draw();
    } else if (croupier_score > player_score) {
      Loose();
    } else {
      Win();
    }
  }
}
