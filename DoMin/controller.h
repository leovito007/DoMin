#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "DEFINE.h"

class Controller {
 public:
  void OnClick()
  {

  }

  // DATA
  int* mineIndex = new int[DEFINE::numMine];
  int* clickedIndex = new int[DEFINE::numRow * DEFINE::numRow];
  int* flaggedIndex = new int[DEFINE::numRow * DEFINE::numRow];
};

#endif