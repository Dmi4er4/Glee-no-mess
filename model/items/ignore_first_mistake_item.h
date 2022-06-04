#pragma once

#include "item.h"
#include "model.h"

class IgnoreFirstMistakeItem : public Item {
 public:
  IgnoreFirstMistakeItem();
  void MistakeTrigger() override;
};

