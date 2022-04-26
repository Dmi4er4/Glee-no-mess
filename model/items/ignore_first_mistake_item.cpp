#include "ignore_first_mistake_item.h"

IgnoreFirstMistakeItem::IgnoreFirstMistakeItem() :
    Items("ignore_first_mistake") {
}

void IgnoreFirstMistakeItem::MistakeTrigger() {
  Model::ForgiveFirstMistake();
}
