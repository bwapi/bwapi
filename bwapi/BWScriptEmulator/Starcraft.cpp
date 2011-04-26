#include "Starcraft.h"

DWORD dwRandomSeed;
DWORD dwLastRandom;

short Random()
{
  dwLastRandom = 0x15A4E35 * dwLastRandom + 1;
  return HIWORD(dwLastRandom);
}

short Random(int min, int max)
{
  return min + (Random() * (max - min + 1) >> 15);
}


