#include "graphics.h"

Graphics::Graphics()
{
 screen = window.GetScreen();
 surface = NULL;
 sourcePosition.x = sourcePosition.y = position.x = position.y = 0;
}

void Graphics::LoadImage(const char* path)
{
 if(surface != NULL)
   Free();

 SDL_Surface *tmp = IMG_Load(path);
 surface = SDL_DisplayFormat(tmp);
 SDL_FreeSurface(tmp);
}

void Graphics::Free()
{
 if(surface != NULL)
   SDL_FreeSurface(surface);
}

void Graphics::SetX(int x)
{
 position.x = x;
}

void Graphics::SetY(int y)
{
 position.y = y;
}

int Graphics::GetX()
{
 return position.x;
}

int Graphics::GetY()
{
  return position.y;
}

void Graphics::Draw()
{
 SDL_BlitSurface(surface, NULL, screen, &position);
}

void Graphics::DrawClip()
{
    SDL_BlitSurface(surface, &sourcePosition, screen, &position);
}

void Graphics::SetSource(int x, int y)
{
    sourcePosition.x = x;
    sourcePosition.y = y;
}

void Graphics::SetRectangle(int w, int h) //used to create a rectangle
{
    position.w = w;
    position.h = h;
}

void Graphics::SetFillColor(int r, int g, int b)
{
    colorKey = SDL_MapRGB(screen->format, r, g, b);
}

void Graphics::Fill() //also Draws to screen
{
    SDL_FillRect(screen, &position, colorKey);
}

Graphics::~Graphics()
{
 Free();
}
