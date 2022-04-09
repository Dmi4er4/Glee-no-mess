#pragma once

#include "random.h"

#include <QGraphicsItem>

#include <memory>

class Guest {
 public:
  Guest();
  ~Guest();

  bool IsMale() const;

  static QRect GetRectWaiting(int index);
  static QRect GetRectActive();

  void SetActive();
  void SetIndex(int index);

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

  auto GetSprite() { return sprite_.get(); }

 private:
  SkinColor skin_color_;
  Sex sex_;
  EyeColor eye_color_;
  Neatness neatness_;
  std::unique_ptr<QGraphicsRectItem> sprite_;
};
