#include "block.h"

Block::Block():LEFT(223), RIGHT(403), BOTTOM(397), TOP(37), CELLW(18), NUM_BLOCKS(7)
{
 gameOver = animateLineComplete = textMotion = false;
 srand(time(0));  //seed the random number generator

 /*//initialize the effect graphics
 effectGfx[0].LoadImage("images/animred.bmp");
 effectGfx[1].LoadImage("images/animwhite.bmp");
 effectGfx[0].SetX(LEFT);
 effectGfx[1].SetX(LEFT);
 */

 topMostRow = 19;
 short tmpArray[7][3][3] = {
                              {{1,1,0},
                               {1,1,0},
                               {0,0,0}},  //square:blockID = 0
                              {{1,1,0},
                               {0,1,1},
                               {0,0,0}}, //z
                              {{0,0,0},
                               {1,1,1},
                               {0,0,0}},  //long bar:blockID = 1
                              {{0,1,1},
                               {1,1,0},
                               {0,0,0}}, //s
                              {{0,0,1},
                               {1,1,1},
                               {0,0,0}}, //L
                              {{0,0,0},
                               {1,1,1},
                               {0,0,1}}, //mirror L
                              {{0,1,0},
                               {1,1,1},
                               {0,0,0}}  //inverted T
                            };



//now initialize the blocksArray
 for(short i = 0; i < NUM_BLOCKS; i++)
    for(short j = 0; j < 3; j++)
        for(short k = 0; k < 3; k++)
            blocksArray[i][j][k] = tmpArray[i][j][k];

 GridSetup();
 InitNextBlocksGfx();
 DropNextBlock();
}

void Block::InitNextBlocksGfx()
{
    MAXNEXTBLOCKS = 4;
    nextCount = 0;
    //initially filling the nextBlocks array
    for(short i = 0; i < MAXNEXTBLOCKS; i++)
        nextBlockID[i] = rand() % NUM_BLOCKS;

    int xLeft = 460, yTop = 70, CELLW_SMALL = 10;
    int spacing[3] = {CELLW, CELLW_SMALL, CELLW_SMALL};
    int x[3] = {xLeft, xLeft + CELLW * 3 + CELLW_SMALL};
    x[2] = x[1] + CELLW_SMALL * 4;
    int y[3] = {yTop, yTop + CELLW_SMALL * 2, yTop + CELLW_SMALL * 4};

    short i, j, k;
        for (i = 0; i < (MAXNEXTBLOCKS - 1); i++)
        for(j = 0; j < 3; j++)
        for(k = 0; k < 3; k++)
        {
            nextBlockGfx[i][j][k].SetX(x[i] + spacing[i] * k);
            nextBlockGfx[i][j][k].SetY(y[i] + spacing[i] * j);
            if(i != 0)                          //0 is already initialized
                nextBlockGfx[i][j][k].LoadImage("images//cell_small.bmp");
        }

}

void Block::UpdateNextBlocksGfx()
{
    short i, j, k;
    short next = nextCount + 1;

    for(i = 0; i < (MAXNEXTBLOCKS - 1); i++)
    {
        if(next > (MAXNEXTBLOCKS - 1))
            next = 0;

        for(j = 0; j < 3; j++)
        {
            for(k = 0; k < 3; k++)
            {
                nextBlockGfx[i][j][k].MakeVisible(blocksArray[nextBlockID[next]][j][k]);
            }
        }
        next++;
    }
}

void Block::DropNextBlock()
{
    UpdateNextBlocksGfx();
    moveLeft = moveRight = true;
    ChooseBlock();
    SetBlockInitialPosition();
}

void Block::ChooseBlock()
{
    blockID = nextBlockID[nextCount];//give a random number from 0 to 6
    nextBlockID[nextCount] = rand() % NUM_BLOCKS; //update the current array
    nextCount++;
    if(nextCount > (MAXNEXTBLOCKS - 1))  //only 3 guesses max
    {
        nextCount = 0;
    }

    short toggle;

    for(short i = 0; i < 3; i++)
        for(short j = 0; j < 3; j++)
        {
            toggle = blocksArray[blockID][i][j];
            currentBlock[i][j] = toggle;
            cells[i][j].MakeVisible(toggle);
        }

    SetBlockBoundary(); //set the visible coords of the block
                        //changes are made to leftRow,leftCol and so on
}

void Block::SetBlockInitialPosition()
{
 int x = LEFT + CELLW * 4;
 int y = TOP - CELLW * 2;

 for(int i = 0; i < 3; i++)
   for(int j = 0; j < 3; j++)
     {
      cells[i][j].SetX(x + CELLW * j);
      cells[i][j].SetY(y + CELLW * i);
     }
}

void Block::SetBlockBoundary()
{
    //for left boundary
    for(short i = 0; i < 3; i++)
        for(short j = 0; j < 3; j ++)
        {
            if(currentBlock[j][i])  //1 or 0 analogous to if(cells[j][i].IsVisible())
            {
                leftRow = j;
                leftCol = i;

                                //right boundary check (0,2), (1,2) ,(2,2), (0,1) and so on
                     for(short i = 2; i >= 0; i--)
                        for(short j = 0; j < 3; j ++)
                        {
                            if(currentBlock[j][i])
                            {
                                rightRow = j;
                                rightCol = i;

                                    //bottom boundary...start from bottom..move right then to up
                                            for(short i = 2; i >= 0; i--)
                                                for(short j = 0; j < 3; j ++)
                                                {
                                                    if(currentBlock[i][j])
                                                    {
                                                        bottomRow = i;
                                                        bottomCol = j;
                                                        return;
                                                    }
                                                }
                            }
                        }
            }
        }

}

void Block::Move()
{
    short i, j;
    HandleEvents();
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            {
                cells[i][j].Move();
            }
}

void Block::RotateBlock()
{
    if(blockID == 0)        //no need to rotate the square block
        return;

    //also if a block is in mid between two grid with a single line space betn them, dont rotate
    short i, j, row, col;
    bool test1, test2, stopChecking = false;

    for(i = bottomRow;(!stopChecking && i >= 0); i--)
        for(j = 0; j < 3; j++)
        {
            if(currentBlock[i][j])
            {
                GetGridRowCol(i, j, row, col);
                if(col > 0 && col < 19)
                {
                    test1 = gridMatrix[row][col - 1] && gridMatrix[row][col + 1];
                    test2 = (gridMatrix[row + 1][col - 1] && gridMatrix[row + 1][col + 1]) &&
                            (cells[i][j].GetY() + CELLW > grid[row + 1][col -1].GetY());
                    if(test1 || test2)
                        return;
                    else if(gridMatrix[row + 1][col - 1] || gridMatrix[row + 1][col + 1])
                    {
                        bool stop = false;
                        for(short r = 0;(!stop && r < 3); r++)
                            for(short cl = 0; cl < 3; cl++)
                            {
                                if(currentBlock[r][cl])
                                {
                                    short row1, col1;
                                    GetGridRowCol(r, cl, row1, col1);
                                    if(col1 > 1 && col1 < 18)
                                    {
                                        if(gridMatrix[row + 1][col - 1] &&
                                            (!gridMatrix[row1][col1 - 1] || !gridMatrix[row1][col1 - 2]))
                                                return;
                                        else if(gridMatrix[row + 1][col + 1] &&
                                                (!gridMatrix[row1][col1 + 1] || !gridMatrix[row1][col1 + 2]))
                                                    return;
                                    }

                                    stop = true;
                                    break;
                                }
                            }
                    }
                }
                stopChecking = true;
                break;
            }
        }

    //else transpose the current block but moving from right to left
    short tmpRow, tmpCol, toggle;
    short tmpMatrix[3][3];
    stopChecking = false;

    for(i = 2, tmpRow = 0; i >= 0; i--, tmpRow++)
        for(j = tmpCol = 0; j < 3; j++, tmpCol++)
            tmpMatrix[tmpRow][tmpCol] = currentBlock[j][i];

            //transformation formula ..clockwise
            //[a00][a01][a02]   [a02][a12][a22]
            //[a10][a11][a12] = [a01][a11][a21]
            //[a20][a21][a22]   [a00][a10][a20]

    //check if the rotation causes the block to go out of boundary
        //check in order [0][2], [1][2], [2][2]... for RIGHT boundary
    for(i = 2; (!stopChecking && i >= 0); i--)
        for(j = 0; j < 3; j++)
            {

                if(tmpMatrix[j][i])  //means if this cell is on the verge of being visible
                {
                    if(cells[j][i].GetX() >= RIGHT)  //means if crossed the RIGHT
                        return;                    //get out. no rotation
                    stopChecking = true;
                    break;
                }
            }

    stopChecking = false;
    //check for left boundary in order [0][0], [1][0], [2][0] and so on
    for(i = 0;(!stopChecking && i < 3); i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(tmpMatrix[j][i])
            {
                if(cells[j][i].GetX() < LEFT)
                    return;
                stopChecking = true;
                break;
            }
        }
    }

    //check if illegal rotation inside the grid
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
        {
            if(tmpMatrix[i][j])
            {
                GetGridRowCol(i, j, row, col);
                if(gridMatrix[row][col])  //if a visible block is on the same place in grid
                    return;
            }
        }


    //now update the currentBlock Matrix
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            {
                toggle = tmpMatrix[i][j];
                currentBlock[i][j] = toggle;
                cells[i][j].MakeVisible(toggle);        //also update the cells to be visible
            }
    SetBlockBoundary();     //and define new Block boundary
    moveLeft = moveRight = true; //needed for some cases
}

void Block::CheckBoundary()
{
    if(cells[bottomRow][bottomCol].GetY() + CELLW >= BOTTOM)
    {
        RespondToKeyPresses(); //respond to further key presses
        UpdateGrid();
        CheckLineComplete();
        DropNextBlock();
        return;
    }
    else if(cells[leftRow][leftCol].GetX() <= LEFT)
    {
            moveLeft = false;
    }
    else if(cells[rightRow][rightCol].GetX() + CELLW >= RIGHT)
    {
            moveRight = false;
    }

    //now check if the block collides with the grid
    short i, j, row, col;
    //First Check if the block is above the topmost row
    GetGridRowCol(bottomRow, bottomCol, row, col);
    if(row + 1 < topMostRow) //the bottom visible row of block is less than topmost row on grid
        return;

    //for bottom check
    for(i = bottomRow; i >=0; i--)
        for(j = 0; j < 3; j++)
            {
                if(currentBlock[i][j])  //means the current cell on bottom is visible
                {
                    //now check if there is a visible grid below it
                    //first find the row and column wrt grid of the visible cell of currentBlock

                    GetGridRowCol(i, j, row, col);
                    if(gridMatrix[row + 1][col])  //means there is a visible block on the grid so drop next block
                        {
                            RespondToKeyPresses(); //if a left or right key is still pressed respond to it

                            //if still there is a gap below after responding to key press
                            //then move down
                            bool moveDown = true;

                            for( i = bottomRow; i >= 0 && moveDown; i--)
                                for( j = 0; j < 3; j++)
                                {
                                    if(currentBlock[i][j])
                                    {
                                        GetGridRowCol(i, j, row, col);
                                        if(row < 19)
                                        {
                                            if(gridMatrix[row + 1][col])
                                                moveDown = false;
                                        }
                                    }
                                }


                            if(!moveDown)
                            {
                            UpdateGrid();
                            CheckLineComplete();
                            DropNextBlock();
                            }
                            return;
                        }

                }
            }
}

void Block::RespondToKeyPresses()
{
    Uint8* keystate;
    keystate = SDL_GetKeyState(NULL);
    while(keystate[SDLK_LEFT] && CanMoveLeft())
    {
        keystate = SDL_GetKeyState(NULL);
        SetCelldx(-18);
        Move();
    }

    while(keystate[SDLK_RIGHT] && CanMoveRight())
    {
        keystate = SDL_GetKeyState(NULL);
        SetCelldx(18);
        Move();
    }
}

bool Block::CanMoveLeft()
{
    short i, j, row, col;
    bool test1 = false, test2 = false, result;

    for(i = leftCol; i < 3; i++)    //move [0][2]..[0][1]..[0][0]..[1][2]..and son on
        for(j = 2; j >=0; j--)
        {
            if(currentBlock[j][i])
            {
                GetGridRowCol(j, i, row, col);
                if(row < 19 && col > 0)
                    test1 = gridMatrix[row + 1][col - 1] && (cells[j][i].GetY() + CELLW > grid[row + 1][col - 1].GetY());
                if(col > 0)
                    test2 = gridMatrix[row][col - 1];

                if(test1 || test2)
                    return false;
                else
                    result = true;
            }

        }
return result;
}

bool Block::CanMoveRight()
{
    short i, j, row, col;
    bool test1 = false, test2 = false, result;

    for(i = rightCol; i >=0; i--)  //move [2][2]..[2][1]..[2][0]..[1][2]..and so on
        for(j = 2; j >= 0; j--)
        {
            if(currentBlock[j][i])
            {
                GetGridRowCol(j, i, row, col);

                if(row < 19 && col < 9)
                test1 = gridMatrix[row + 1] [ col + 1] && (cells[j][i].GetY() + CELLW > grid[row + 1][col + 1].GetY());
                if(col < 9)
                test2 = gridMatrix[row][col + 1];

                if(test1 || test2)
                    return false;
                else
                    result = true;
            }
        }
return result;
}
void Block::DrawBlock()
{
 for(short i = 0; i < 3; i++)
   for(short j = 0; j < 3; j++)
     {
        if(currentBlock[i][j])
            cells[i][j].Draw();
     }
}

void Block::DrawNextBlocks()
{
    short i, j, k;
    for(i = 0; i < (MAXNEXTBLOCKS - 1); i++)
    for(j = 0; j < 3; j++)
    for(k = 0; k < 3; k++)
    {
        if(nextBlockGfx[i][j][k].IsVisible())
            nextBlockGfx[i][j][k].Draw();
    }
}

void Block::SetCellVelocity(float value)
{
    Cell::SetVelocity(value);
}

void Block::IncreaseDefaultVelocity()
{
    Cell::SetDEFAULTVELOCITY(Cell::DEFAULTVELOCITY() + 1);
}

void Block::SetCelldx(int value)
{
    for(short i = 0; i < 3; i++)
    for(short j = 0; j < 3; j++)
     cells[i][j].Setdx(value);
}

void Block::GridSetup()
{
    for(short i = 0; i < 20; i++)
        for(short j =0; j < 10; j++)
        {
            gridMatrix[i][j] = 0;
            grid[i][j].MakeVisible(0);
            grid[i][j].SetX(LEFT + j * CELLW);
            grid[i][j].SetY(TOP + i * CELLW);
        }
}

short Block::GetGridMatrix(short row, short col)
{
    return gridMatrix[row][col];
}

void Block::SetGridMatrix(short row, short col)
{
    //remember row and col must be within boundary
    gridMatrix[row][col] = 1;
}

void Block::UpdateGrid()
{
    short row, col;

    for(short i = 0; i < 3; i++)
        for(short j = 0; j < 3; j++)
        {
            if(currentBlock[i][j])  //if the cell of the current block is visible, store it in grid
            {
                GetGridRowCol(i, j, row, col);      //row, col is modified
                gridMatrix[row][col] = 1;
            }
        }

    topMostRow = FindTopRow();       //update the top most row
    if(topMostRow == 0) gameOver = true; //game is over if first row is touched
}

short Block::FindTopRow()
{
    //first check if the middle row..i.e the 9th row is filled
    short i, j, MID = 9, COLS = 10, ROWS = 20;
    bool middleFilled = false;

    for(j = 0; j < COLS; j++)
        if(gridMatrix[MID][j])        //if any grid is filled on middle row
        {
            middleFilled = true;
            break;
        }

    if(middleFilled)        //means topmost row is on middle or above it
    {
        for(i = 0; i <= MID; i++)
            for(j = 0; j < COLS; j++)
            {
                if(gridMatrix[i][j])
                 return i;              //return the ROW number
            }
    }
    else        //check from middle + 1 to bottom..since MID is not filled
    {
        for(i = MID + 1; i < ROWS; i++)
            for(j = 0; j < COLS; j++)
            {
                if(gridMatrix[i][j])
                    return i;
            }
    }
return ROWS - 1; //if all grid is empty return the last row
}

void Block::CheckLineComplete()
{
    //move from bottom row wise to topMostRow
    short i = 19, number; //the bottom row
    while(i >= topMostRow)
    {
        if(IsRowFilled(i, number))
        {
            if(number > 1)
                SetTextMotion(number);

            SetAnimation(i, number);
            DropGrid(i, number);    //drop the grid from topMostRow to i th row 'number' tim
            topMostRow += number;  //the topMostrow will increase by 1..ie goes down
            if(topMostRow > 19) topMostRow = 19; //may be sometimes
            i++;            //stay on the current row..since it again needs to be checked
        }
        i--;
    }
}

bool Block::IsRowFilled(short rowNum, short &number)
{
    short column;
    number = 0;
    for(column = 0; column < 10; column++)
    {
        if(gridMatrix[rowNum][column] == 0) //if it is not filled
            return false;
    }

    //now count the number of rows contiguously filled
    number = 1;
    column = 0;
    if (rowNum > 1)
    {
        rowNum --;
        while(gridMatrix[rowNum][column++] && rowNum > 1) //no need to check row Number 0
        {
            if(column > 9)
            {
                number++; //an extra row is filled upwards
                rowNum--; //move upwards
                column = 0;
            }
        }
    }
    return true;
}

void Block::SetTextMotion(short numberOfLines)
{
    ANIM_NUMOFLINES = numberOfLines;
    textMotion = true;
}

bool Block::DoTextMotion(short &numberOfLines) //accessed by Game::Process
{
    if(textMotion)
    {
        numberOfLines = ANIM_NUMOFLINES;
        textMotion = false;
        return true;
    }
    else
     return false;
}

void Block::SetAnimation(short startRow, short amount) //blinking while line complete
{
    ANIM_StartRow = startRow;
    ANIM_Amount = amount;
    animateLineComplete = true;
}

bool Block::DoAnimation(short &startRow, short &amount) //accessed by Game::Process
{
    if(animateLineComplete)
    {
        startRow = ANIM_StartRow;
        amount = ANIM_Amount;

        animateLineComplete = false;    //prevent further animation
        return true;
    }
    else
        return false;
}

void Block::DropGrid(short startRow, short amount)
{
    short row = startRow, col, endRow = topMostRow + amount;
    if(endRow <= 19) //else delete all
    {
        while(row >= endRow) //from startRow to endRow
        {
            for(col = 0; col < 10; col++)
                gridMatrix[row][col] = gridMatrix[row - amount][col]; //copy elements from above row

            row--;
        }
    }
    //delete top most rows = amount
    //move from topMostRow towards bottom

    for(row = topMostRow; row < endRow; row ++)
        for(col = 0; col < 10; col++)       //delete the elements on topMostRow
            gridMatrix[row][col] = 0;
}

void Block::DrawGrid()
{
    for(short i = 0; i < 20; i++)
        for(short j = 0; j < 10; j++)
        {
            if(gridMatrix[i][j])
                grid[i][j].Draw();
        }

}

void Block::GetGridRowCol(short i, short j, short& row, short& col)
{
    row = (cells[i][j].GetY() - TOP) / 18;
    col = (cells[i][j].GetX() - LEFT) / 18;
}

void Block::HandleEvents()
{
  while(SDL_PollEvent(&event))
  {
   switch(event.type)
    {
     case SDL_KEYDOWN:
     {
         switch(event.key.keysym.sym)
         {
          case SDLK_LEFT:
          {
                if(moveLeft && CanMoveLeft())
                    SetCelldx(-18);

               moveRight = true;
               break;
          }
          case SDLK_RIGHT:
          {
               if(moveRight && CanMoveRight())
                        SetCelldx(18);

               moveLeft = true;
               break;
          }
          case SDLK_DOWN:
          {
               SetCellVelocity(12);
               break;
          }
         }//2nd switch ends
         break;
     }

     case SDL_KEYUP:
     {
       SDLKey k = event.key.keysym.sym;
       if(k == SDLK_DOWN)
       {
           SetCellVelocity(Cell::DEFAULTVELOCITY());
       }
       else if(k == SDLK_UP)
       {
           RotateBlock();
       }
      break;
     }
    }//first switch ends
  }//while ends
}//fn ends

bool Block::IsGameOver()
{
    return gameOver;
}
