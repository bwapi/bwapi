#include "ScriptThread.h"
#include "ScriptEnum.h"
#include "BWScriptEmulator.h"

#include "Controller.h"

#include <BWAPI.h>

using namespace BWAPI;

#define LOCATION_SIZE 1024

#define MACRO_BUILD   0
#define MACRO_UPGRADE 1
#define MACRO_TECH    2

std::list<aithread> aiThreadList;

BYTE *pbAIScriptBinary;

#define AIS_ENTRY_REQUIRES_LOCATION   0x01
#define AIS_ENTRY_STAREDIT_INVISIBLE  0x02
#define AIS_ENTRY_REQUIRES_BROODWAR   0x04

struct ais_entry
{
  DWORD dwScriptID;
  DWORD dwOffset;
  DWORD dwStringIndex;
  DWORD dwFlags;
};

int threadCount = 0;
void UpdateScripts()
{
  std::list<aithread>::iterator i = aiThreadList.begin();
  while ( i != aiThreadList.end() )
  {
    i->showDebug(0, 0);
    if ( !i->sleep() )    // wait/sleep
    {
      if ( i->getFlags() & AI_THREAD_WANT_KILLED )
      {
        i = aiThreadList.erase(i);
        --threadCount;
        continue;
      }      

      // execute thread
      i->execute();
    }
    ++i;
  }
}

bool LoadAIBinary(const char *pszFileName)
{
  HANDLE hFile = CreateFileA(pszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  if ( hFile == INVALID_HANDLE_VALUE )
    return false;

  DWORD dwFileSize = GetFileSize(hFile, NULL);
  if ( dwFileSize <= 10 )
  {
    CloseHandle(hFile);
    return false;
  }
  pbAIScriptBinary = (BYTE*)malloc(dwFileSize);
  if ( !pbAIScriptBinary )
  {
    CloseHandle(hFile);
    return false;
  }
  memset(pbAIScriptBinary, 0, dwFileSize);

  DWORD dwBytesRead = 0;
  ReadFile(hFile, pbAIScriptBinary, dwFileSize, &dwBytesRead, NULL);
  CloseHandle(hFile);
  return true;
}

void AICreateThread(WORD wStartBlock, Position location, void *pTown)
{
  aiThreadList.push_front(aithread(wStartBlock, location, pTown));
  ++threadCount;
}

void AICreateThread(const char *pszScriptID, Position location)
{
  if ( !pszScriptID || strlen(pszScriptID) < 4 || !pbAIScriptBinary )
    return;

  DWORD dwScriptID = *(DWORD*)pszScriptID;

  ais_entry *entryList = (ais_entry*)(pbAIScriptBinary + *(DWORD*)pbAIScriptBinary);
  DWORD dwScriptOffset = 0;
  for ( int i = 0; entryList[i].dwScriptID != 0 && dwScriptOffset == 0; ++i )
  {
    if ( dwScriptID == entryList[i].dwScriptID )
    {
      dwScriptOffset = entryList[i].dwOffset;
      break;
    }
  }

  if ( dwScriptOffset )
  {
    AICreateThread((WORD)dwScriptOffset, location);
    Broodwar->sendText("Beginning AI script execution: %s", pszScriptID);
  }
  else
    Broodwar->sendText("Unable to find script: %s", pszScriptID);
}

aithread::aithread()
:dwScriptOffset(0)
,dwSleepTime(0)
,dwPlayerID( (Broodwar->self()->getID()) )
,pTown(nullptr)
,dwFlags(0)
,threadId(threadCount)
,dwBytesRead(0)
,retryBlock(false)
{
  memset(this->bTotBuildCount, 0, sizeof(this->bTotBuildCount));
  this->debugQueue.clear();
}

aithread::aithread(WORD wStartBlock, BWAPI::Position location, void *town)
:dwScriptOffset(wStartBlock)
,dwSleepTime(0)
,dwPlayerID(Broodwar->self()->getID())
,pTown(town)
,dwFlags(0)
,threadId(threadCount)
,dwBytesRead(0)
,retryBlock(false)
{
  BWAPI::Position size(LOCATION_SIZE,LOCATION_SIZE);
  this->location = Location(location - size, location + size);
  memset(this->bTotBuildCount, 0, sizeof(this->bTotBuildCount));
  this->debugQueue.clear();
}

aithread::~aithread()
{
  for ( std::deque<char*>::iterator c = this->debugQueue.begin(); c != this->debugQueue.end(); ++c )
    free(*c);
  this->debugQueue.clear();
}

int GetStandardUnitCount(UnitType type, bool bCompleted, Player player)
{
  int count = 0;
  Player pl = (player ? player : self);
  if ( !pl )
    return 0;
  if ( bCompleted )
  {
    count = pl->completedUnitCount(type);
    if ( type == UnitTypes::Terran_Siege_Tank_Siege_Mode )
      count += pl->completedUnitCount(UnitTypes::Terran_Siege_Tank_Tank_Mode);
    else if ( type == UnitTypes::Terran_Siege_Tank_Tank_Mode )
      count += pl->completedUnitCount(UnitTypes::Terran_Siege_Tank_Siege_Mode);
  }
  else
  {
    count = pl->allUnitCount(type);
    if ( type == UnitTypes::Terran_Siege_Tank_Siege_Mode )
      count += pl->allUnitCount(UnitTypes::Terran_Siege_Tank_Tank_Mode);
    else if ( type == UnitTypes::Terran_Siege_Tank_Tank_Mode )
      count += pl->allUnitCount(UnitTypes::Terran_Siege_Tank_Siege_Mode);
  }
  return count;
}

DWORD aithread::getFlags() const
{
  return this->dwFlags;
}

void aithread::setFlags(DWORD dwFlags)
{
  this->dwFlags |= dwFlags;
}

void aithread::clearFlags(DWORD dwFlags)
{
  this->dwFlags &= ~dwFlags;
}

DWORD aithread::sleep()
{
  return this->dwSleepTime ? this->dwSleepTime-- : 0;
}

void aithread::setSleep(DWORD dwSleepAmt)
{
  this->dwSleepTime += dwSleepAmt;
}

void aithread::killThread()
{
  this->setFlags(AI_THREAD_WANT_KILLED);
}

void aithread::showDebug(int x, int y)
{
  int _y = y;
  int _x = x + this->threadId*200 + 4;
  bw->setTextSize(Text::Size::Small);
  for each ( char *s in this->debugQueue )
  {
    char n[512];
    strcpy_s(n, 512, s);
    
    char *mid = strchr(n, ' ');
    if ( mid )
    {
      *mid = '\0';
      ++mid;
    }
    
    bw->drawTextScreen(_x, _y, "%s", n);
    if ( mid )
      bw->drawTextScreen(_x+100, _y, "%s", mid);
    _y += 9;
  }
  bw->setTextSize();
}

void aithread::saveDebug(const char prefix, int iOpcode, const char *pszFormat, ...)
{
  if ( retryBlock )
    return;

  char szFinalBuffer[512];
  char szBuffer[256];

  szFinalBuffer[0] = 0;
  szBuffer[0]      = 0;

  if ( pszFormat )
  {
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf_s(szBuffer, 256, 256, pszFormat, ap);
    va_end(ap);
  }
  sprintf_s(szFinalBuffer, 512, "%c%s %s", prefix, AISCRIPT::getOpcodeName(iOpcode), szBuffer);

  char *tmp = (char*)malloc(strlen(szFinalBuffer)+1);
  strcpy_s(tmp, strlen(szFinalBuffer)+1, szFinalBuffer);

  if ( this->debugQueue.size() > 32 )
  {
    char *front = this->debugQueue.front();
    free(front);
    this->debugQueue.pop_front();
  }
  this->debugQueue.push_back(tmp);
}
bool aithread::retry()
{
  this->dwScriptOffset -= this->dwBytesRead;
  this->setSleep(30);
  this->dwBytesRead     = 0;
  this->retryBlock      = true;
  return false;
}
bool aithread::noretry()
{
  this->dwBytesRead = 0;
  this->retryBlock  = false;
  return true;
}

void aithread::setScriptOffset(DWORD dwNewOffset)
{
  this->dwScriptOffset = dwNewOffset;
}
DWORD aithread::getScriptOffset() const
{
  return this->dwScriptOffset;
}

Location aithread::getLocation()
{
  return this->location;
}

void aithread::call(DWORD dwOffset)
{
  this->callstack.push( this->getScriptOffset() );
  this->setScriptOffset(dwOffset);
}
bool aithread::ret()
{
  if ( this->callstack.empty() )
    return false;

  this->setScriptOffset( this->callstack.top() );
  this->callstack.pop();
  return true;
}

void *aithread::town()
{
  return this->pTown;
}

