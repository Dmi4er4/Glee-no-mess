#include "ignore_first_mistake_item.h"

IgnoreFirstMistakeItem::IgnoreFirstMistakeItem() :
    Item("ignore_first_mistake") {
}

void IgnoreFirstMistakeItem::MistakeTrigger() {
  Model::ForgiveFirstMistake();
}
