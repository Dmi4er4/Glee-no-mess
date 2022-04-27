#pragma once

#include "item.h"
#include "model.h"
#include "file_loader.h"

class TimeItem : public Item {
 public:
  TimeItem();

  void TimeTrigger() override;

 private:
  size_t time_;
};
