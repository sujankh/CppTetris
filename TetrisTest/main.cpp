#include <SDL/SDL.h>

#include "screen.h"
#include "game.h"

Screen window;

int main(int argc, char* argv[])
{
 Uint32 mode=SDL_SWSURFACE | SDL_FULLSCREEN;

 const int WIDTH = 640, HEIGHT = 480;

 window.SetVideo(WIDTH, HEIGHT, 32, mode);
 Game testMode;
 testMode.Init("Tetris By Sujan");
 testMode.Process();
 return 0;
}
