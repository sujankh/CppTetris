#include "cell.h"
//definition of static variables in Cell classes
float Cell::VELOCITY, Cell::DEFAULTVEL;

Cell::Cell(char* path) : Grid(path)
{
 dx = dy = 0;
 SetDEFAULTVELOCITY(0.5);
}

void Cell::Setdx(int value)
{
  dx = value;
}

void Cell::Setdy(int value)
{
    dy = value;
}

void Cell::Move()
{
 MoveX();
 MoveY();
}

void Cell::MoveX()
{
 int x = GetX();


 if(dx != 0)
 {
   SetX(x + (int) dx);
   dx = 0;
 }
}

void Cell::MoveY()
{
 int y = GetY();
 dy += VELOCITY;

 y +=(int) dy;
 if(dy >= 1) dy = 0;

 SetY(y);
}

void Cell::SetVelocity(float vel)
{
  VELOCITY = vel;
}

void Cell::SetDEFAULTVELOCITY(float vel)
{
    SetVelocity(vel);
    DEFAULTVEL = vel;
}

float Cell::DEFAULTVELOCITY()
{
    return DEFAULTVEL;
}

