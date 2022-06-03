#include "guest.h"

#include "view.h"
#include "model.h"
#include "level.h"

Guest::~Guest() {
  View::Instance().GetGameScene()->removeItem(sprite_.get());
}

void Guest::SetAnimation(const QString& filename) {
  animation_ = std::make_unique<PolylineAnimation>(filename);
  ReloadPosition();
}

void Guest::SetSprite(QGraphicsPixmapItem* sprite) {
  sprite_.reset(sprite);
}

void Guest::ReloadPosition() {
  auto [x, y] = animation_->Position();
  sprite_->setPos(x, y - sprite_->boundingRect().height());
}

bool Guest::DoAnimation(int millis, int index_in_queue) {
  auto res = animation_->Do(millis, index_in_queue);
  ReloadPosition();
  return res;
}
