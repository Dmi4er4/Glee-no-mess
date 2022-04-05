#include "guest.h"

// Guest::Guest() : skin_color_(SkinColor::kBlack),
//                      sex_(Sex::kFemale),
//                      eye_color_(EyeColor::kBlue),
//                      neatness_(Neatness::kState1) {
//
// }

Guest::Guest() : random_engine_(std::random_device()()),
                 skin_color_(static_cast<SkinColor>(
                                     std::uniform_int_distribution<int>(0, 1)(
                                         random_engine_))),
                 sex_(static_cast<Sex>(
                              std::uniform_int_distribution<int>(0, 1)(
                                  random_engine_))),
                 eye_color_(static_cast<EyeColor>(
                                    std::uniform_int_distribution<int>(0, 2)(
                                        random_engine_))),
                 neatness_(static_cast<Neatness>(
                                   std::uniform_int_distribution<int>(0, 2)(
                                       random_engine_))) {
}

bool Guest::Good() const {
  return sex_ == Sex::kMale;
}
