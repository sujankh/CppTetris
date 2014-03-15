#include "background.h"

Background::Background()
{
}

Background::Background(char* path)
{
  LoadImage(path);
}
