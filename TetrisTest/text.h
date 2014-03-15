#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "screen.h"
extern Screen window;

/**specification
First set the font size
then the font type
then the style
and finally set the text
otherwise u get a default type and size
*/

class Text
{
    private:
     SDL_Surface *text; //the surface to hold text
     SDL_Rect position;
     TTF_Font *font;    //manages fonts of the text
     SDL_Color textColor;
     int fontSize;

    public:
    Text(char* inputText = "TEXT", int x = 0, int y = 0);
    ~Text();

    void SetText(const char* inputText = "TEXT");
    void SetFontType(char* fontType = "fonts//font1900.ttf");
    void SetFontSize(int size);
    void MakeBold();
    void SetColor(int r = 255, int g = 0, int b = 0);

    static void Init(); //to be called before any object is created
    static void Close();//to be called after all objects are destroyed
    void Free();

    void SetX(int x);
    void SetY(int y);
    void SetXY(int x, int y);
    int GetX();
    int GetY();

    void Show();
};

#endif // TEXT_H_INCLUDED
