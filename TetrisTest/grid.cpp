#include "grid.h"

Grid::Grid(char* path)
{
 LoadImage(path);
}

void Grid::MakeVisible(bool vis)
{
    visible = vis;
}

bool Grid::IsVisible()
{
    return visible;
}
