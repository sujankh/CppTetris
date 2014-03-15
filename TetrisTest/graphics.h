#ifndef GRAPHI_H
#define GRAPHI_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "screen.h"
extern Screen window;

class Graphics
{
 private:
   SDL_Surface *surface, *screen;
   SDL_Rect position, sourcePosition;
   Uint32 colorKey;
 public:
   Graphics();
   ~Graphics();
   void LoadImage(const char* path);
   void Free();
   void SetX(int x);
   void SetY(int x);

   void Draw();

   void DrawClip();
   void SetSource(int x, int y);

   void SetRectangle(int w, int h);
   void SetFillColor(int r, int g, int b);
   void Fill();

   int GetX();
   int GetY();
 };

#endif
