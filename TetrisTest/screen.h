#ifndef SCREEN_H
#define SCREEN_H

#include <SDL/SDL.h>

class Screen
{
 private:
  SDL_Surface *screen;

 public:
   void SetVideo(int W, int H, int bit, Uint32 mode);
   void Flip();
   SDL_Surface* GetScreen();
};

#endif
