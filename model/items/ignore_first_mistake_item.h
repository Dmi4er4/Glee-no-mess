#pragma once

#include "items.h"
#include "model.h"

class IgnoreFirstMistakeItem : public Items {
 public:
  IgnoreFirstMistakeItem();
  void MistakeTrigger() override;
};

