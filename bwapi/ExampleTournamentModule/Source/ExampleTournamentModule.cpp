#include <windows.h>
#include <Shlwapi.h>
#include "ExampleTournamentModule.h"
using namespace BWAPI;

void ExampleTournamentAI::onStart()
{
}

void ExampleTournamentAI::onEnd(bool isWinner)
{
}

void ExampleTournamentAI::onFrame()
{
}

void ExampleTournamentAI::onSendText(std::string text)
{
}

void ExampleTournamentAI::onReceiveText(BWAPI::Player* player, std::string text)
{
}

void ExampleTournamentAI::onPlayerLeft(BWAPI::Player* player)
{
}

void ExampleTournamentAI::onNukeDetect(BWAPI::Position target)
{
}

void ExampleTournamentAI::onUnitDiscover(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitEvade(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitShow(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitHide(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitCreate(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitDestroy(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitMorph(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitRenegade(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onSaveGame(std::string gameName)
{
}

// Use some existing Battle.net filters
const char *pszBadWords[] =
{ 
  "asshole",
  "bitch",
  "clit",
  "cock",
  "cunt",
  "dick",
  "dildo",
  "faggot",
  "fuck",
  "gook",
  "masturbat",
  "nigga",
  "nigger",
  "penis",
  "pussy",
  "shit",
  "slut",
  "whore",
  NULL
};

const char szBadWordCharacters[] = { '!', '@', '#', '$', '%', '&' };

void BadWordFilter(char *pszString)
{
  // Iterate each badword
  for ( int f = 0; pszBadWords[f]; ++f )
  {
    // Find badword
    char *pszMatch = StrStrI(pszString, pszBadWords[f]);
    if ( !pszMatch )
      continue; // continue if badword not found

    // iterate characters in badword
    char cLast = 0;
    for ( int i = 0; pszBadWords[f][i]; ++i )
    {
      // make the character compatible with our replacements
      int val = pszBadWords[f][i] & 7;
      if ( val >= sizeof(szBadWordCharacters) )
        val = 0;

      // increment the replacement if it's the same as our last one, reset to 0 if it's out of bounds
      if ( cLast == szBadWordCharacters[val] && ++val == sizeof(szBadWordCharacters) )
        val = 0;

      // apply our change to the original string and save the last character used
      pszMatch[i] = szBadWordCharacters[val];
      cLast       = szBadWordCharacters[val];
    }
  }
}

bool ExampleTournamentModule::onPrintf(char *pszString)
{
  BadWordFilter(pszString);
  return true;
}
bool ExampleTournamentModule::onSendText(char *pszString)
{
  BadWordFilter(pszString);
  return true;
}
bool ExampleTournamentModule::onAction(int actionType, int parameter)
{
  return true; // Not yet implemented
}
bool ExampleTournamentModule::onDraw(int shape, int ctype, int x1, int y1, int x2, int y2, int x3, int y3, BWAPI::Color color, bool isSolid)
{
  return true;  // Allow AI module to draw shapes and figures
}
