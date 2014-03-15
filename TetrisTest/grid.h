#ifndef GRID_H
#define GRID_H

#include "graphics.h"

class Grid : public Graphics
{
 private:
  bool visible;

 public:
  Grid(char* path = "images//cell.bmp");
  void MakeVisible(bool vis = true);
  bool IsVisible();
};
#endif // GRID_H
