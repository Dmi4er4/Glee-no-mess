#pragma once

#include <QString>

#include <vector>

constexpr size_t kMaxScore = 21;

const std::vector<QString> kSuitsNames{"clubs", "dimonds", "spades", "hearts"};
const std::vector<QString> kValueNames{"2", "3", "4", "5", "6", "7", "8", "9",
                                       "10", "Jack", "Queen", "King", "Ace"};

namespace cards {
enum class Suit {
  kClub,
  kDimond,
  kSpade,
  kHeart,
  kCount
};

enum class Value {
  kTwo,
  kThree,
  kFour,
  kFive,
  kSix,
  kSeven,
  kEight,
  kNine,
  kTen,
  kJack,
  kQueen,
  kKing,
  kAce,
  kCount
};

const std::vector<size_t> kWeights =
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

struct Cards {
  Cards(Suit suit, Value value) :
    suit(suit), value(value), weight(kWeights[static_cast<int>(value)]) {
  }

  Suit suit;
  Value value;
  size_t weight;
};

}  // namespace cards
