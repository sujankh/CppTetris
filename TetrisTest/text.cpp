#include "text.h"

Text::Text(char* inputText, int x, int y)
{
    text = NULL;
    fontSize = 25;
    SetFontType();
    SetColor();
    SetText(inputText);
    SetXY(x, y);
}

Text::~Text()
{
    TTF_CloseFont(font);
}

void Text::Free()
{
    if(text != NULL)
        SDL_FreeSurface(text);
}

void Text::Init()
{
    if(!TTF_WasInit())
    {
        TTF_Init();
    }
}

void Text::Close()
{
    TTF_Quit();
}

void Text::SetText(const char* inputText)
{
    Free();
    text = TTF_RenderText_Solid(font, inputText, textColor);
}

void Text::SetFontType(char* fontType) //set type as well as size
{
    font = TTF_OpenFont(fontType, fontSize);
}

void Text::SetFontSize(int size)
{
    fontSize = size;
}

void Text::MakeBold()
{
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
}

void Text::SetColor(int r, int g, int b)
{
    textColor.r = r;
    textColor.g = g;
    textColor.b = b;
}

void Text::SetX(int x)
{
    position.x = x;
}

void Text::SetY(int y)
{
    position.y = y;
}

void Text::SetXY(int x, int y)
{
    SetX(x);
    SetY(y);
}

int Text::GetX()
{
 return position.x;
}

int Text::GetY()
{
  return position.y;
}

void Text::Show()
{
    SDL_BlitSurface(text, NULL, window.GetScreen(), &position);
}

