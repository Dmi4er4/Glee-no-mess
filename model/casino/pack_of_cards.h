#pragma once

#include <vector>

#include "cards.h"

class PackOfCards {
 public:
  PackOfCards();
  cards::Cards GetRandomCard();

 private:
  std::vector<cards::Cards> cards_{};
};
