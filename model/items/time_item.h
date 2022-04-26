#pragma once

#include "items.h"
#include "model.h"
#include "file_loader.h"

class TimeItem : public Items {
 public:
  TimeItem();

  void TimeTrigger() override;

 private:
  size_t time_;
};
