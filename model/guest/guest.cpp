#include "guest.h"

#include "view.h"
#include "model.h"

Guest::Guest()
    : skin_color_(Random::EnumChoose<guest_traits::SkinColor>()),
      sex_(Random::EnumChoose<guest_traits::Sex>()),
      eye_color_(Random::EnumChoose<guest_traits::EyeColor>()),
      neatness_(Random::EnumChoose<guest_traits::Neatness>()),
      bloody_(Random::EnumChoose<guest_traits::Bloody>()),
      sprite_(new QGraphicsRectItem) {
  sprite_->setBrush(IsMale()
                    ? View::kBluePolygonBrush
                    : View::kYellowPolygonBrush);
  View::Instance().GetScene()->addItem(sprite_.get());
}

Guest::~Guest() {
  View::Instance().GetScene()->removeItem(sprite_.get());
}

bool Guest::IsMale() const {
  return sex_ == guest_traits::Sex::kMale;
}

void Guest::SetActive() {
  sprite_->setRect(GetRectActive());
}

void Guest::SetIndex(int index) {
  sprite_->setRect(GetRectWaiting(index));
}

QRect Guest::GetRectWaiting(int index) {
  return {
      View::Instance().width() - (View::kSquareSize + View::kMargin) *
          (Model::kQueueLength - index),
      (View::Instance().height() + View::kSquareSize) / 2,
      View::kSquareSize,
      View::kSquareSize,
  };
}

QRect Guest::GetRectActive() {
  return {
      View::kMargin,
      (View::Instance().height() + View::kSquareSize) / 2,
      View::kSquareSize,
      View::kSquareSize,
  };
}
