#pragma once

#include <QGraphicsItem>

#include <memory>

#include "random.h"
#include "guest_traits.h"
#include "polyline_animation.h"

class Guest {
 public:
  Guest();
  ~Guest();

  bool IsMale() const;

  void SetActive();
  void SetIndex(int index);

  static QRect GetRectWaiting(int index);
  static QRect GetRectActive();

  auto GetSprite() { return sprite_.get(); }

 private:
  guest_traits::SkinColor skin_color_;
  guest_traits::Sex sex_;
  guest_traits::EyeColor eye_color_;
  guest_traits::Neatness neatness_;
  guest_traits::Bloody bloody_;
  std::unique_ptr<QGraphicsRectItem> sprite_;
};
