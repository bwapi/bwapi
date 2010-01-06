#include <stdio.h>
#include <tchar.h>

#include <windows.h>

#include <BWAPI\all.h>

#include <string>

using namespace BWAPI;

int _tmain(int argc, _TCHAR* argv[])
{
  const StaticGameData *gd;
  if(!Connect())
  {
    printf("trying to connect...\n");
    while(!Connect())
    {
      Sleep(1000);
    }
  }
  printf("connected\n");

  gd = GetStaticGameData();

  for(;;)
  {
    WaitForEvent();
    int count = 0;
    for each(Util::Indexed<const KnownUnit&> kunit in gd->units)
    {
      count++;
    }
    char buff[100];
    itoa(count, buff, 10);
    DrawText(10, 10, buff);
  }

  system("pause");
  return 0;
}

