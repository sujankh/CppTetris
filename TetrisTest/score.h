#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <cstdio>
using std::sprintf;

#include <string>
using std::string;

#include "text.h"
class Score
{
    private:
        int SCORELIST[3];
        long totalScore;
        int level, linesCompleted; //must reach 15 in each level to advance to next
        int lines[3];
        //score board
        Text scoreBoard[12];
        char tmpTXT[6][20];
    public:
        Score();
        void AddScore(int numberOfLines); //add scores based on number of lines consumed
        bool NextLevel();

        void InitScoreBoard();
        void DrawScoreBoard();
        void UpdateScoreBoard();
};

#endif // SCORE_H_INCLUDED
