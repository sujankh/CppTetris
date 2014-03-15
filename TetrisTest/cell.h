#ifndef CELL_H
#define CELL_H

#include "grid.h"

class Cell : public Grid  //Cell refers to a moveable grid
{
 private:
  float dx, dy;
  static float VELOCITY;
  static float DEFAULTVEL;
  /*****UTILITIES*****/

 public:
  Cell(char* path = "images//cell.bmp");
  void Setdx(int value);
  void Setdy(int value);
  static void SetVelocity(float vel);
  static void SetDEFAULTVELOCITY(float vel);
  static float DEFAULTVELOCITY();

  void Move();
  void MoveX();
  void MoveY();

  //void MakeVisible(bool vis = true);
  //bool IsVisible();
};
#endif
