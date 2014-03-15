#include "screen.h"

void Screen::SetVideo(int W, int H, int bit, Uint32 mode)
{
   SDL_Init(SDL_INIT_VIDEO);
   screen = SDL_SetVideoMode(W, H, bit, mode);
}

void Screen::Flip()
{
 SDL_Flip(screen);
}

SDL_Surface* Screen::GetScreen()
{
 return screen;
}
