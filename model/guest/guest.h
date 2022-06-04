#pragma once

#include <QGraphicsItem>

#include <memory>

#include "random.h"
#include "guest_traits.h"
#include "polyline_animation.h"

class Level;

class Guest {
 public:
  ~Guest();

  auto GetSprite() { return sprite_.get(); }
  bool IsGood() { return good_; }
  void SetAnimation(const QString& filename);
  void SetSprite(QGraphicsPixmapItem* sprite);
  bool DoAnimation(int millis, int index_in_queue);
  void ReloadPosition();

  auto GetAnimation() { return animation_.get(); }

 private:
  friend class Level;

  bool good_{};
  std::unique_ptr<PolylineAnimation> animation_;
  std::unique_ptr<QGraphicsPixmapItem> sprite_;
};
