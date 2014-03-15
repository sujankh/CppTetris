#include "game.h"

Game::Game():FramePS(60), TOP(37), LEFT(223), CELLW(18), BOTTOM(397)
{
 TICK_INTERVAL = 1000 / FramePS;
}

Game::~Game()
{
 SDL_Quit();
}

void Game::Init(char* title)
{
 SDL_WM_SetCaption(title, NULL);
 SDL_ShowCursor(SDL_ENABLE);
 SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY - 100 , SDL_DEFAULT_REPEAT_INTERVAL - 30);
}

void Game::Process()
{
 Text::Init();  //initialize text..calling a static function

 Uint8* keystate;
 bool running = true;
 int level = 1;

//paths of backgrounds
 string bgPath[10], bgTopPath[10];

 for(short i = 0; i < 10; i++)
 {
     string num = ToString(i);
     bgPath[i] = "images//bg" + num + ".png";
     bgTopPath[i] = "images//bgtop" + num + ".png";
 }

 Background bg;
 Background bgtop;

 Text welcome("Tetris", 30, 75); //coords also supplied
 Text lineComplete[2];
 short numberOfLines;
 textMotion = false;
 InitTextMotion(lineComplete);

 gfxRect.SetX(LEFT);

 Score scoreManager;
 scoreManager.InitScoreBoard();
 scoreManager.UpdateScoreBoard();

 bg.LoadImage(bgPath[0].c_str());
 bgtop.LoadImage(bgTopPath[0].c_str());

 nextTime = SDL_GetTicks() + TICK_INTERVAL;
 while(running)
 {
    tetris.Move();
    tetris.CheckBoundary();

    bg.Draw();
    welcome.Show();
    scoreManager.DrawScoreBoard();
    tetris.DrawGrid();
    tetris.DrawNextBlocks();
    tetris.DrawBlock();
    bgtop.Draw();

    if(tetris.DoAnimation(startRow, amount))
    {
        //this is true only if a line or more is consumed..so score can be obtained
        scoreManager.AddScore(amount); //amount is the number of lines

        if(scoreManager.NextLevel())
        {
            level++;
            bg.LoadImage(bgPath[level - 1].c_str());
            bgtop.LoadImage(bgTopPath[level - 1].c_str());
            tetris.IncreaseDefaultVelocity();
        }
        scoreManager.UpdateScoreBoard();
        ANIMLineComplete();
        DoLineCompleteAnimation();
    }

    if(tetris.DoTextMotion(numberOfLines))
    {
        lineComplete[numberOfLines - 2].SetY(BOTTOM - CELLW);
        textMotion = true;
    }
    if(textMotion)
        DoTextMotion(lineComplete[numberOfLines - 2]);   //if text motion is enabled then do it

    if(tetris.IsGameOver())
    {
         ShowGameOver();
         running = false;
         continue;
    }

    keystate = SDL_GetKeyState(NULL);
    if(keystate[SDLK_ESCAPE])
    {
       running = false;
       continue;
    }
    else if(keystate[SDLK_p])   //pause
    {
        keystate[SDLK_p] = 0;
        while(1)
        {
            keystate = SDL_GetKeyState(NULL);
            if(keystate[SDLK_p])
             break;
             SDL_PumpEvents();  //update events
             SDL_Delay(60); //put some stop for saving CPU power
        }
        keystate[SDLK_p] = 0;
    }

    window.Flip();
    SDL_Delay(TimeLeft());
    nextTime += TICK_INTERVAL;
 }

 Text::Close();
}

Uint32 Game::TimeLeft()
{
  Uint32 now;

  now = SDL_GetTicks();
  if(nextTime <= now)
      return 0;
  else
      return nextTime - now;
}

void Game::ANIMLineComplete()
{
    int y = TOP + (startRow - amount + 1) * CELLW;

    gfxRect.SetRectangle(CELLW * 10, amount * CELLW);
    gfxRect.SetY(y);
}

void Game::DoLineCompleteAnimation()
{
    short i = 0;
        while(i++ < 5)
        {
            if(i % 2 == 0)
             gfxRect.SetFillColor(255, 0, 0);   //red
            else
             gfxRect.SetFillColor(255, 255, 255);   //white

            gfxRect.Fill();
            window.Flip();
            SDL_Delay(30);
        }
}

void Game::InitTextMotion(Text *lineComplete)
{
     char *messages[] = {"Double Line", "Triple Line"};
     for(short i = 0; i < 2; i++)
     {
         lineComplete[i].SetFontSize(20);
         lineComplete[i].SetColor(0, 0, 255);
         lineComplete[i].SetFontType("fonts//BAUHS93.ttf");
         lineComplete[i].SetText(messages[i]);
         lineComplete[i].SetX(LEFT + CELLW * 2);
     }

}

void Game::DoTextMotion(Text &txt)
{
    int y = txt.GetY();
    txt.SetY(y - 4);
    txt.Show();
    if(y < TOP)
     textMotion = false;
}

void Game::ShowGameOver()
{
     Text gameOver;
     gameOver.SetFontSize(35);
     gameOver.SetColor(200, 0, 255);
     gameOver.SetFontType("fonts//BRADHITC.ttf");
     gameOver.MakeBold();
     gameOver.SetText("Game Over");
     gameOver.SetXY(LEFT, TOP + CELLW * 8);

     nextTime = SDL_GetTicks() + TICK_INTERVAL;

     for(short row = 0; row < 20; row++)
        for(short col = 0; col < 10; col++)
        {
            if(tetris.GetGridMatrix(row,col) == 0)
            {
                tetris.SetGridMatrix(row, col);
                tetris.DrawGrid();
                window.Flip();
            }
            SDL_Delay(TimeLeft());
            nextTime += TICK_INTERVAL;
        }

        gameOver.Show();
        window.Flip();

     while(1)
     {
        if(WaitEvent(SDLK_AMPERSAND)) //any key
            break;
        SDL_Delay(60);
     }
}

bool Game::WaitEvent(SDLKey value) //EVENT HANDLER
{
if(SDL_PollEvent(&event))
  {
   switch(event.type)
    {
     case SDL_KEYDOWN:
     if(value == SDLK_AMPERSAND) return true;
      SDLKey k=event.key.keysym.sym;
      if(k==value)
         {
           return true;
         }
      break;
    }
  }
return false;
}

string Game::ToString(short c)
{
string s,rev;
short r;

do
{
r=c%10;
s+=static_cast<char> (48+r);
c/=10;
}while(c!=0);

for(int i=s.length()-1;i>=0;i--)
        rev+=s[i];
return rev;
}

