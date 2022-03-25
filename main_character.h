#ifndef MAIN_CHARACTER_H_
#define MAIN_CHARACTER_H_

#include <cstdint>
#include <vector>

class MainCharacter {
 public:
  enum class inventory_items {
    kItem1,
    kItem2,
    kItem3,
    kCount
  };

  MainCharacter();

 private:
  int64_t money_{};
  std::vector<bool> inventory_{};

};

#endif  // MAIN_CHARACTER_H_
