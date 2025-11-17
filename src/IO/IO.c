#include "IO.h"
#include <stdlib.h>

void os_rand()
{
  srand(0);
}
int IO_request()
{
  if (rand() % CHANCE_OF_IO_REQUEST == 0)
    return 1;
  else
    return 0;
}
int IO_complete()
{
  if (rand() % CHANCE_OF_IO_COMPLETE == 0)
    return 1;
  else
    return 0;
}