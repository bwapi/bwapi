#pragma once
#include <BWAPI.h>
class TestMap1
{
public:
  void onStart();
  void onEnd(bool isWinner);
  void onFrame();
  void onSendText(std::string text);
  void onPlayerLeft(BWAPI::Player player);
  void onNukeDetect(BWAPI::Position target);
  void onUnitCreate(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void onUnitMorph(BWAPI::Unit unit);
  void onUnitShow(BWAPI::Unit unit);
  void onUnitHide(BWAPI::Unit unit);
  void onUnitRenegade(BWAPI::Unit unit);
  void onSaveGame(std::string gameName);
};