#include "ignore_first_mistake_item.h"

#include "all_items.h"

IgnoreFirstMistakeItem::IgnoreFirstMistakeItem() :
    Item(kIgnoreFirstMistake) {
}

void IgnoreFirstMistakeItem::MistakeTrigger() {
  // Model::Instance().ForgiveFirstMistake();
}
