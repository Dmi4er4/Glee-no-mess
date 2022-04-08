#pragma once

#include "random.h"

class Guest {
 public:
  Guest();

  bool IsMale() const;

 public:
  enum class SkinColor {
    kBlack,
    kWhite,
    kCount
  };

  enum class Sex {
    kMale,
    kFemale,
    kCount
  };

  enum class EyeColor {
    kGreen,
    kBlue,
    kBrown,
    kCount
  };

  enum class Neatness {
    kState1,
    kState2,
    kState3,
    kCount
  };

 private:
  SkinColor skin_color_;
  Sex sex_;
  EyeColor eye_color_;
  Neatness neatness_;
};
