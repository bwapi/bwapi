#include "Actions.h"

#include "FileReader.h"
#include <stdio.h>
#include <BWAPI.h>
/*
int GetActionSize(BYTE *pBuffer)
{
  int iSize = 1;
  switch ( pBuffer[0] )
  {
  case Actions::Save_Game:
  case Actions::Load_Game:
    iSize += 4;
    while ( pBuffer[iSize] )
      ++iSize;
    break;
  case Actions::Chat_Replay:
    iSize += 81;
  case Actions::Select_Delta_Add:
  case Actions::Select_Delta_Del:
  case Actions::Select_Units:
    iSize += 2 * pBuffer[iSize];
    break;
  case Actions::Placebox:
    iSize += 7;
    break;
  case Actions::Set_Fog:
  case Actions::Group_Units:
  case Actions::Train:
  case Actions::Cancel_Train:
  case Actions::Unit_Morph:
  case Actions::Exit_Transport:
  case Actions::Building_Morph:
    iSize += 2;
    break;
  case Actions::Set_Allies:
  case Actions::Cheat:
  case Actions::Lift_Off:
  case Actions::Ping_Minimap:
    iSize += 4;
    break;
  case Actions::Set_Speed:
  case Actions::Stop:
  case Actions::Return:
  case Actions::Cloak_Off:
  case Actions::Cloak_On:
  case Actions::Tank_Siege:
  case Actions::Tank_Unsiege:
  case Actions::Unload_All:
  case Actions::Hold_Position:
  case Actions::Burrow_Down:
  case Actions::Burrow_Up:
  case Actions::Research:
  case Actions::Upgrade:
  case Actions::Voice_Enable:
  case Actions::Voice_Squelch:
  case Actions::Set_Latency:
  case Actions::Leave_Game:
    iSize += 1;
    break;
  case Actions::Right_Click:
  case Actions::Set_Replay_Speed:
    iSize += 9;
    break;
  case Actions::Target_Click:
    iSize += 10;
    break;
  default:
    break;
  }
  return iSize;
}
*/



DWORD g_dwHighestFrame;
void ParseActions(FileReader &fr)
{
  FILE *hFile = fopen("DebugActions.txt", "w");
  while ( !fr.Eof() )
  {
    DWORD dwCurrentFrame  = fr.Read<DWORD>();
    if ( dwCurrentFrame > g_dwHighestFrame )
      g_dwHighestFrame = dwCurrentFrame;

    BYTE  bFrameSize      = fr.Read<BYTE>();
    for ( BYTE i = 0; i < bFrameSize; )
    {
      BYTE bPlayerID = fr.Read<BYTE>();
      BYTE bCommand  = fr.Read<BYTE>();
      i += 2;
      fprintf(hFile, "(P%d) %s: ", bPlayerID, bCommand < Actions::Max ? Actions::pszActionNames[bCommand] : "INVALID");
      switch ( bCommand )
      {
      case Actions::Save_Game:
      case Actions::Load_Game:
        {
          DWORD dwSaveInfo = fr.Read<DWORD>();
          std::string name = fr.ReadCString();
          i += sizeof(dwSaveInfo) + name.length() + 1;
          fprintf(hFile, "0x%p, %s", dwSaveInfo, name.c_str());
        }
        break;
      case Actions::Chat_Replay:
        {
          BYTE bPlayerID    = fr.Read<BYTE>();
          char szMessage[80];
          fr.Read(szMessage, sizeof(szMessage));
          i += sizeof(bPlayerID) + sizeof(szMessage);
          fprintf(hFile, "%u, %s", bPlayerID, szMessage);
        }
        break;
      case Actions::Select_Delta_Add:
      case Actions::Select_Delta_Del:
      case Actions::Select_Units:
        {
          WORD wUnits[12] = { 0 };
          BYTE bUnitCount = fr.Read<BYTE>();

          i += sizeof(bUnitCount) + sizeof(WORD) * bUnitCount;

          if ( bUnitCount > 12 )
            bUnitCount = 12;
          fprintf(hFile, "%u", bUnitCount);
          for ( BYTE i = 0; i < bUnitCount; ++i )
          {
            wUnits[i] = fr.Read<WORD>();
            fprintf(hFile, ", (%u, 0x%02X)", wUnits[i] & 0x7FF, wUnits[i] >> 12);
          }
          
        }
        break;
      case Actions::Placebox:
        {
          BWAPI::Order orderType(fr.Read<BYTE>());
          short x = fr.Read<short>();
          short y = fr.Read<short>();
          BWAPI::UnitType unitType(fr.Read<WORD>());
          fprintf(hFile, "%s, (%d, %d), %s", orderType.c_str(), x, y, unitType.c_str());
          i += 7;
        }
        break;
      case Actions::Set_Fog:
        {
          WORD wVision = fr.Read<WORD>();
          fprintf(hFile, "%04X", wVision);
          i += sizeof(wVision);
        }
        break;
      case Actions::Group_Units:
        {
          BYTE bGroupType = fr.Read<BYTE>();
          BYTE bGroupNum  = fr.Read<BYTE>();

          fprintf(hFile, "%s, %d", bGroupType < 3 ? Actions::pszGroupType[bGroupType] : "INVALID", bGroupNum );
          i += sizeof(bGroupType) + sizeof(bGroupNum);
        }
        break;
      case Actions::Train:
      case Actions::Unit_Morph:
      case Actions::Building_Morph:
        {
          BWAPI::UnitType unitType(fr.Read<WORD>());
          fprintf(hFile, "%s", unitType.c_str());
          i += 2;
        }
        break;
      case Actions::Cancel_Train:
      case Actions::Exit_Transport:
        {
          WORD wUnitID = fr.Read<WORD>();
          fprintf(hFile, "(%u, 0x%02X)", wUnitID & 0x7FF, wUnitID >> 12);
          i += 2;
        }
        break;
      case Actions::Set_Allies:
        {
          DWORD dwAllies = fr.Read<DWORD>();
          fprintf(hFile, "0x%08X", dwAllies);
          i += 4;
        }
        break;
      case Actions::Cheat:
        {
          DWORD dwCheatFlags = fr.Read<DWORD>();
          fprintf(hFile, "0x%08X", dwCheatFlags);
          i += 4;
        }
        break;
      case Actions::Lift_Off:
      case Actions::Ping_Minimap:
        {
          short x = fr.Read<short>();
          short y = fr.Read<short>();
          fprintf(hFile, "(%d, %d)", x, y);
          i += 4;
        }
        break;
      case Actions::Set_Speed:
        {
          BYTE bSpeed = fr.Read<BYTE>();
          fprintf(hFile, "%s", bSpeed < 7 ? Actions::pszGameSpeed[bSpeed] : "INVALID");
          i += sizeof(bSpeed);
        }
        break;
      case Actions::Stop:
      case Actions::Return:
      case Actions::Cloak_Off:
      case Actions::Cloak_On:
      case Actions::Tank_Siege:
      case Actions::Tank_Unsiege:
      case Actions::Unload_All:
      case Actions::Hold_Position:
      case Actions::Burrow_Down:
      case Actions::Burrow_Up:
        {
          BYTE bHow = fr.Read<BYTE>();
          fprintf(hFile, "%s", bHow ? "Queued" : "");
          i += sizeof(bHow);
        }
        break;
      case Actions::Research:
        {
          BWAPI::TechType tech(fr.Read<BYTE>());
          fprintf(hFile, "%s", tech.c_str());
          i += 1;
        }
        break;
      case Actions::Upgrade:
        {
          BWAPI::UpgradeType upgrade(fr.Read<BYTE>());
          fprintf(hFile, "%s", upgrade.c_str());
          i += 1;
        }
        break;
      case Actions::Voice_Enable:
      case Actions::Voice_Squelch:
        {
          BYTE bPlayerID = fr.Read<BYTE>();
          fprintf(hFile, "%d", bPlayerID);
          i += sizeof(bPlayerID);
        }
        break;
      case Actions::Set_Latency:
        {
          BYTE bLatency = fr.Read<BYTE>();
          fprintf(hFile, "%s", bLatency < 3 ? Actions::pszLatency[bLatency] : "INVALID");
          i += sizeof(bLatency);
        }
        break;
      case Actions::Leave_Game:
        {
          BYTE bLeaveType = fr.Read<BYTE>();
          fprintf(hFile, "%s", bLeaveType < 6 ? Actions::pszLeaveType[bLeaveType] : "UNKNOWN");
          i += sizeof(bLeaveType);
        }
        break;
      case Actions::Right_Click:
        {
          short x = fr.Read<short>();
          short y = fr.Read<short>();
          WORD wTargetID = fr.Read<WORD>();
          BWAPI::UnitType unitType(fr.Read<WORD>());
          BYTE bHow = fr.Read<BYTE>();
          fprintf(hFile, "(%d, %d), (%u, 0x%02X), %s%s", x, y, wTargetID & 0x7FF, wTargetID >> 12, unitType.c_str(), bHow ? ", Queued" : "");
          i += 9;
        }
        break;
      case Actions::Set_Replay_Speed:
        {
          bool paused = fr.Read<bool>();
          DWORD dwSpeed = fr.Read<DWORD>();
          DWORD dwMultiplier = fr.Read<DWORD>();
          fprintf(hFile, "%s%s, ×%d", paused ? "PAUSED, " : "", dwSpeed < 7 ? Actions::pszGameSpeed[dwSpeed] : "INVALID", dwMultiplier);
          i += 9;
        }
        break;
      case Actions::Target_Click:
        {
          short x = fr.Read<short>();
          short y = fr.Read<short>();
          WORD wTargetID = fr.Read<WORD>();
          BWAPI::UnitType unitType(fr.Read<WORD>());
          BWAPI::Order orderType(fr.Read<BYTE>());
          BYTE bHow = fr.Read<BYTE>();
          fprintf(hFile, "(%d, %d), (%u, 0x%02X), %s, %s%s", x, y, wTargetID & 0x7FF, wTargetID >> 12, unitType.c_str(), orderType.c_str(), bHow ? ", Queued" : "");
          i += 10;
        }
        break;
      default:
        break;
      } // switch
      fprintf(hFile, "\n");
    } // for
  } // while
  if ( hFile )
    fclose(hFile);
}
