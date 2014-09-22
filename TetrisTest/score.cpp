#include "score.h"

Score::Score()
{
    SCORELIST[0] = 275; //for single line for 1st level
    SCORELIST[1] = 550; //double
    SCORELIST[2] = 1100; //triple

    totalScore = 0;
    level = 1;
    linesCompleted = 0;
    lines[0] = lines[1] = lines[2] = 0;

}

void Score::AddScore(int numberOfLines)
{
    totalScore += SCORELIST[numberOfLines - 1] * level;
    linesCompleted += numberOfLines;

    lines[numberOfLines - 1]++; //update singles, doubles, triples
}

bool Score::NextLevel()
{
    if(linesCompleted == 0) return false;
    if(linesCompleted % 15 == 0) //if yes jump to next level
    {
        level++;
        return true;
    }
    else
        return false;
}

void Score::InitScoreBoard()
{
    char* txt[] = {"Level", "x", "Score", "x", "Singles", "x", "Doubles", "x", "Triples",
                 "x", "Total", "x"};
    int coordsx[] = {30, 465, 570};
    int coordsy = 150, YINCR = 0, YINCR1 = 0;

    for(short i = 0; i < 12; i++)
    {
        if(i % 2 == 0) //for texts
        {
            scoreBoard[i].SetFontSize(20);
            scoreBoard[i].SetFontType("fonts//font1900.ttf");
            scoreBoard[i].SetColor(255, 0, 0);
        }
        else        //for the numbers
        {
            scoreBoard[i].SetFontType("fonts//BRADHITC.TTF");
            scoreBoard[i].MakeBold();
            scoreBoard[i].SetFontSize(15);
            scoreBoard[i].SetColor(255, 255, 255);
        }

        if(i <= 3)
        {
            scoreBoard[i].SetXY(coordsx[0], coordsy + YINCR);
            YINCR += 25;
        }
        else
        {
            if(i % 2 == 0)
                scoreBoard[i].SetXY(coordsx[1], coordsy + YINCR1);
            else
            {
                scoreBoard[i].SetXY(coordsx[2], coordsy + YINCR1);
                YINCR1 += 25;
            }

        }

        scoreBoard[i].SetText(txt[i]);
    }
}

void Score::UpdateScoreBoard()
{
    long scores[] = {level, totalScore, lines[0], lines[1], lines[2], linesCompleted};
    for(short i = 1, j = 0; i < 12; i += 2, j++)
    {
        sprintf(tmpTXT[j], "%ld", scores[j]);
        scoreBoard[i].SetText(tmpTXT[j]);
    }
}

void Score::DrawScoreBoard()
{
    for(short i = 0; i < 12; i++)
        scoreBoard[i].Show();
}
