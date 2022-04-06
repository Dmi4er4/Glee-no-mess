#pragma once

#include "random.h"

class Guest {
 public:
  Guest();

  bool IsMale() const;

 public:
  enum class SkinColor {
    kBlack,
    kWhite
  };

  enum class Sex {
    kMale,
    kFemale
  };

  enum class EyeColor {
    kGreen,
    kBlue,
    kBrown
  };

  enum class Neatness {
    kState1,
    kState2,
    kState3
  };

 private:
  Random rand;
  SkinColor skin_color_;
  Sex sex_;
  EyeColor eye_color_;
  Neatness neatness_;
};
