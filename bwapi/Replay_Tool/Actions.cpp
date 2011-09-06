#include "Actions.h"

#include "FileReader.h"
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
  while ( !fr.Eof() )
  {
    DWORD dwCurrentFrame  = fr.Read<DWORD>();
    if ( dwCurrentFrame > g_dwHighestFrame )
      g_dwHighestFrame = dwCurrentFrame;

    BYTE  bFrameSize      = fr.Read<BYTE>();
    for ( BYTE i = 0; i < bFrameSize; ++i )
    {
      BYTE bPlayerID = fr.Read<BYTE>();
      switch ( fr.Read<BYTE>() )
      {
      case Actions::Save_Game:
      case Actions::Load_Game:
        {
          DWORD dwSaveInfo = fr.Read<DWORD>();
          std::string name = fr.ReadCString();
          i += sizeof(dwSaveInfo) + name.length() + 1;
        }
        break;
      case Actions::Chat_Replay:
        {
          BYTE bPlayerID    = fr.Read<BYTE>();
          char szMessage[80];
          fr.Read(szMessage, sizeof(szMessage));
          i += sizeof(bPlayerID) + sizeof(szMessage);
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
          for ( BYTE i = 0; i < bUnitCount; ++i )
            wUnits[i] = fr.Read<WORD>();
        }
        break;
      case Actions::Placebox:
        {
          BWAPI::Order orderType(fr.Read<BYTE>());
          short x = fr.Read<short>();
          short y = fr.Read<short>();
          BWAPI::UnitType unitType(fr.Read<WORD>());
          i += 7;
        }
        break;
      case Actions::Set_Fog:
        {
          WORD wVision = fr.Read<WORD>();
          i += sizeof(wVision);
        }
        break;
      case Actions::Group_Units:
        {
          BYTE bGroupType = fr.Read<BYTE>();
          BYTE bGroupNum  = fr.Read<BYTE>();
          i += sizeof(bGroupType) + sizeof(bGroupNum);
        }
        break;
      case Actions::Train:
      case Actions::Unit_Morph:
      case Actions::Building_Morph:
        {
          BWAPI::UnitType unitType(fr.Read<WORD>());
          i += 2;
        }
        break;
      case Actions::Cancel_Train:
      case Actions::Exit_Transport:
        {
          WORD wUnitID = fr.Read<WORD>();
          i += 2;
        }
        break;
      case Actions::Set_Allies:
        {
          DWORD dwAllies = fr.Read<DWORD>();
          i += 4;
        }
        break;
      case Actions::Cheat:
        {
          DWORD dwCheatFlags = fr.Read<DWORD>();
          i += 4;
        }
        break;
      case Actions::Lift_Off:
      case Actions::Ping_Minimap:
        {
          short x = fr.Read<short>();
          short y = fr.Read<short>();
          i += 4;
        }
        break;
      case Actions::Set_Speed:
        {
          BYTE bSpeed = fr.Read<BYTE>();
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
          i += sizeof(bHow);
        }
        break;
      case Actions::Research:
        {
          BWAPI::TechType tech(fr.Read<BYTE>());
          i += 1;
        }
        break;
      case Actions::Upgrade:
        {
          BWAPI::UpgradeType upgrade(fr.Read<BYTE>());
          i += 1;
        }
        break;
      case Actions::Voice_Enable:
      case Actions::Voice_Squelch:
        {
          BYTE bPlayerID = fr.Read<BYTE>();
          i += sizeof(bPlayerID);
        }
        break;
      case Actions::Set_Latency:
        {
          BYTE bLatency = fr.Read<BYTE>();
          i += sizeof(bLatency);
        }
        break;
      case Actions::Leave_Game:
        {
          BYTE bLeaveType = fr.Read<BYTE>();
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
          i += 9;
        }
        break;
      case Actions::Set_Replay_Speed:
        {
          bool paused = fr.Read<bool>();
          DWORD dwSpeed = fr.Read<DWORD>();
          DWORD dwMultiplier = fr.Read<DWORD>();
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
          i += 10;
        }
        break;
      default:
        break;
      } // switch
    } // for
  } // while
}
