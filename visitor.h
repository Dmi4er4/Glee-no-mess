#ifndef VISITOR_H_
#define VISITOR_H_

#include <random>

class Visitor {
 public:
  Visitor();

  bool Good() const;

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
  std::default_random_engine random_engine_;
  SkinColor skin_color_;
  Sex sex_;
  EyeColor eye_color_;
  Neatness neatness_;
};

#endif  // VISITOR_H_
