#pragma once

#include <Util/Types.h>

namespace BW
{
  /**
   * Direct mapping of bw tech identification. 
   * Source is http://www.staredit.net/wiki/Techdata.dat_Entry_Listing
   */
  namespace TechID
  {
    enum Enum : u8
    {
      Stimpacks         = 0x00,
      Lockdown          = 0x01,
      EMPShockwave      = 0x02,
      SpiderMines       = 0x03,
      ScannerSweep      = 0x04,
      TankSiegeMode     = 0x05,
      DefensiveMatrix   = 0x06,
      Irradiate         = 0x07,
      YamatoGun         = 0x08,
      CloakingField     = 0x09,
      PersonnelCloaking = 0x0A,
      Burrowing         = 0x0B,
      Infestation       = 0x0C,
      SpawnBroodlings   = 0x0D,
      DarkSwarm         = 0x0E,
      Plague            = 0x0F,
      Consume           = 0x10,
      Ensnare           = 0x11,
      Parasite          = 0x12,
      PsionicStorm      = 0x13,
      Hallucination     = 0x14,
      Recall            = 0x15,
      StasisField       = 0x16,
      ArchonWarp        = 0x17,
      Restoration       = 0x18,
      DisruptionWeb     = 0x19,
      UnusedTech26      = 0x1A,
      MindControl       = 0x1B,
      DarkArchonMeld    = 0x1C,
      Feedback          = 0x1D,
      OpticalFlare      = 0x1E,
      Maelstorm         = 0x1F,
      LurkerAspect      = 0x20,
      UnusedTech33      = 0x21,
      Healing           = 0x22,
      UnusedTech35      = 0x23,
      UnusedTech36      = 0x24,
      UnusedTech37      = 0x25,
      UnusedTech38      = 0x26,
      UnusedTech39      = 0x27,
      UnusedTech40      = 0x28,
      UnusedTech41      = 0x29,
      UnusedTech42      = 0x2A,
      UnusedTech43      = 0x2B,
      None              = 0x2C
    };
  }
}