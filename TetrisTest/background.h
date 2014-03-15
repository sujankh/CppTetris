#ifndef BACKGR_H
#define BACKGR_H

#include "graphics.h"

class Background:public Graphics
{
 public:
  Background();
  Background(char* path);
};
#endif
