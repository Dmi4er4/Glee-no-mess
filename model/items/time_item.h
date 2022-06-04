#pragma once

#include "item.h"
#include "model.h"

class TimeItem : public Item {
 public:
  TimeItem();

  void TimeTrigger() override;

 private:
  size_t time_;
};
