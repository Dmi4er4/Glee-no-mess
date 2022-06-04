#include "pack_of_cards.h"

#include "random.h"

PackOfCards::PackOfCards() {
  for (int i = 0; i < static_cast<int>(cards::Suit::kCount); ++i) {
    for (int j = 0; j < static_cast<int>(cards::Value::kCount); ++j) {
      cards_.emplace_back(static_cast<cards::Suit>(i),
                          static_cast<cards::Value>(j));
    }
  }
}

cards::Cards PackOfCards::GetRandomCard() {
  size_t num = Random::RandomInt(0, cards_.size() - 1);
  cards::Cards card = cards_[num];
  cards_.erase(cards_.begin() + num);
  return card;
}
