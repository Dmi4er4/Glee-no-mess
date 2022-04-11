#include "guest.h"

#include "view.h"
#include "model.h"

Guest::Guest()
    : skin_color_(Random::EnumChoose<VisitorTraits::SkinColor>()),
      sex_(Random::EnumChoose<VisitorTraits::Sex>()),
      eye_color_(Random::EnumChoose<VisitorTraits::EyeColor>()),
      neatness_(Random::EnumChoose<VisitorTraits::Neatness>()),
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
  return sex_ == VisitorTraits::Sex::kMale;
}

void Guest::SetActive() {
  sprite_->setRect(GetRectActive());
}

void Guest::SetIndex(int index) {
  sprite_->setRect(GetRectWaiting(index));
}

QRect Guest::GetRectWaiting(int index) {
  return {
      View::kWidth - (View::kSquareSize + View::kMargin) *
          (Model::kQueueLength - index),
      (View::kHeight + View::kSquareSize) / 2,
      View::kSquareSize,
      View::kSquareSize,
  };
}

QRect Guest::GetRectActive() {
  return {
      -View::kMargin,
      (View::kHeight + View::kSquareSize) / 2,
      View::kSquareSize,
      View::kSquareSize,
  };
}
