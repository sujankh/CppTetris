#ifndef BLOCK_H
#define BLOCK_H

#include <ctime>
using std::time;

#include <cstdlib>
using std::rand;
using std::srand;

#include "grid.h"
#include "cell.h"

class Block
{
 private:
  const int LEFT, RIGHT, BOTTOM, TOP, CELLW;
  const short NUM_BLOCKS;
  short blockID;
  short blocksArray[7][3][3];  // 7 =NUM_BLOCKS
  short currentBlock[3][3];    //holds the matrix for currently displayed block
  Cell cells[3][3];
  short leftRow, leftCol, rightRow, rightCol, bottomRow, bottomCol;  //extreme visible coords of a block
                                                                     //like [0][0], [2][2],etc.
                                                                     //set during block select & rotation
  bool moveLeft, moveRight;  //checks if the blocks is allowed to move left or right
  short gridMatrix[20][10];
  Grid grid[20][10];
  short topMostRow;
  SDL_Event event;


  /* for effects */
  short ANIM_StartRow, ANIM_Amount, ANIM_NUMOFLINES; //these are to be passed to Game::Process for blink anim
  bool animateLineComplete, textMotion;

  Grid nextBlockGfx[3][3][3]; //three blocks each 3 x 3
  short nextBlockID[4];
  short nextCount;
  short MAXNEXTBLOCKS;

  //game handling
  bool gameOver;
  /*******UTILITIES******/

  void InitNextBlocksGfx();
  void UpdateNextBlocksGfx();

  void ChooseBlock();
  void SetBlockInitialPosition();
  void SetBlockBoundary();
  void DropNextBlock();
  void RotateBlock();
  bool CanMoveLeft();
  bool CanMoveRight();
  void RespondToKeyPresses();

  void SetCelldx(int value);

  void GridSetup();
  void UpdateGrid();
  short FindTopRow();
  void CheckLineComplete();
  bool IsRowFilled(short rowNum, short &number);
  void DropGrid(short startRow, short amount);
  void GetGridRowCol(short i, short j, short& row, short& col);

  void SetAnimation(short startRow, short amount);
  void SetTextMotion(short numberOfLines);
 public:
  Block();
  void HandleEvents();
  void CheckBoundary();
  void Move();
  void DrawBlock();
  void DrawGrid();
  void DrawNextBlocks();

  bool IsGameOver();
  bool DoAnimation(short &startRow, short &amount);
  bool DoTextMotion(short &numberOfLines); //accessed by Game::Process

  short GetGridMatrix(short row, short col); //setting individual cell of Grid
  void SetGridMatrix(short row, short col);

  void SetCellVelocity(float value);
  void IncreaseDefaultVelocity();
};
#endif
