#include "IO.h"
#include <stdlib.h>

void os_srand(int i)
{
  srand(i);
}
int os_rand(){
  return rand();
}

int IO_request()
{
  if (os_rand() % CHANCE_OF_IO_REQUEST == 0)
    return 1;
  else
    return 0;
}
int IO_complete()
{
  if (os_rand() % CHANCE_OF_IO_COMPLETE == 0)
    return 1;
  else
    return 0;
}