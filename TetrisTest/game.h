#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>

#include <string>
using std::string;

#include "screen.h"
extern Screen window;

#include "graphics.h"
#include "background.h"
#include "block.h"

#include "text.h"
#include "score.h"

class Game
{
 private:
   SDL_Event event;
   const int FramePS;
   int TICK_INTERVAL;
   Uint32 nextTime;

   //For Effects
   const int TOP, LEFT, CELLW, BOTTOM;
   short startRow, amount;
   bool textMotion;

   //Components of Game
   Block tetris;
   Graphics gfxRect;  //blinking effect

   /*UTILITIES*/
   Uint32 TimeLeft();
   void ANIMLineComplete();
   void DoLineCompleteAnimation();

   void InitTextMotion(Text *lineComplete);
   void DoTextMotion(Text &txt);
   void ShowGameOver();
   bool WaitEvent(SDLKey value); //EVENT HANDLER

   string ToString(short c);
 public:
   Game();
   ~Game();
   void Init(char* title);
   void Process();
};
#endif
