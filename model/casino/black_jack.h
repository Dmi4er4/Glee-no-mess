#pragma once

#include <vector>

#include "pack_of_cards.h"

class BlackJack {
 public:
  static BlackJack& Instance();
  void StartNewGame(size_t bid);

  void HitPlayer();
  void HitCroupier();

 private:
  BlackJack() = default;
  int32_t GetScore(const std::vector<cards::Cards>& card_set);
  void ShowNextPlayerCard();
  void ShowNextCroupierCard();
  QString GetCardFileName(const cards::Cards& card);

  void CheckStatus();

  void Draw();
  void Loose();
  void Win();

  PackOfCards pack_of_cards_{};
  std::vector<cards::Cards> croupier_cards_{};
  std::vector<cards::Cards> player_cads_{};

  size_t number_player_card_{};
  size_t number_croupier_card_{};
  size_t bid_{};
};
