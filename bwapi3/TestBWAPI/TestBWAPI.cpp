#include <stdio.h>
#include <tchar.h>

#include <BWAPI\all.h>

#include <windows.h>

#include <string>

using namespace BWAPI;

int _tmain(int argc, _TCHAR* argv[])
{
  if(!Connect())
  {
    printf("trying to connect...\n");
    while(!Connect())
    {
      Sleep(1000);
    }
  }
  printf("connected\n");

  system("pause");
  return 0;
}

