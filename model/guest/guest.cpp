#include "guest.h"

Guest::Guest() : skin_color_(Random::EnumChoose<SkinColor>()),
                 sex_(Random::EnumChoose<Sex>()),
                 eye_color_(Random::EnumChoose<EyeColor>()),
                 neatness_(Random::EnumChoose<Neatness>()) {
}

bool Guest::IsMale() const {
  return sex_ == Sex::kMale;
}
