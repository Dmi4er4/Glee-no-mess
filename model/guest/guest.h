#pragma once

#include "random.h"
#include "visitor_traits.h"

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
  VisitorTraits::SkinColor skin_color_;
  VisitorTraits::Sex sex_;
  VisitorTraits::EyeColor eye_color_;
  VisitorTraits::Neatness neatness_;
  std::unique_ptr<QGraphicsRectItem> sprite_;
};
