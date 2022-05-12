#pragma once

#include <QString>

#include <vector>

const QString kTwo = "2";
const QString kThree = "3";
const QString kFour = "4";
const QString kFive = "5";
const QString kSix = "6";
const QString kSeven = "7";
const QString kEight = "8";
const QString kNine = "9";
const QString kTen = "10";
const QString kJack = "Jack";
const QString kQueen = "Queen";
const QString kKing = "King";
const QString kAce = "Ace";

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
