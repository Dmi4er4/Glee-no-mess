#pragma once

#include "random.h"
#include "guest_traits.h"

#include <QGraphicsItem>

#include <memory>

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
  std::unique_ptr<QGraphicsRectItem> sprite_;
};
