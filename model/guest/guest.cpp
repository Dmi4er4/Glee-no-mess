#include "guest.h"

Guest::Guest() : skin_color_(static_cast<SkinColor>(rand(0, 1))),
                 sex_(static_cast<Sex>(rand(0, 1))),
                 eye_color_(static_cast<EyeColor>(rand(0, 2))),
                 neatness_(static_cast<Neatness>(rand(0, 2))) {
}

bool Guest::IsMale() const {
  return sex_ == Sex::kMale;
}
