#pragma once
#include <array>

namespace BW
{
  const int PLAYER_COUNT = 12;
  const int PLAYABLE_PLAYER_COUNT = 8;
  const int RACE_COUNT = 3;
  const int UNIT_TYPE_COUNT = 228;
  const int TECH_TYPE_COUNT = 44;
  const int UPGRADE_TYPE_COUNT = 61;
  const int WEAPON_TYPE_COUNT = 130;
  const int DAMAGE_TYPE_COUNT = 5;
  const int EXPLOSION_TYPE_COUNT = 25;
  const int FLINGY_TYPE_COUNT = 209;
  const int UNIT_ARRAY_MAX_LENGTH = 1700;
  const int BULLET_ARRAY_MAX_LENGTH = 100;
  const int MAX_SEARCH = (UNIT_ARRAY_MAX_LENGTH * 2);
  const int NUM_SPEEDS = 7;
  const int TURN_BUFFER_SIZE = 512;
  const int NUM_TEXT_LINES = 13;
  const int MAX_TEXT_SIZE = 218;
  const int MAX_SELECTION_COUNT = 12;

  const std::array<unsigned, NUM_SPEEDS> OriginalSpeedModifiers = { 167, 111, 83, 67, 56, 48, 42 };

}
