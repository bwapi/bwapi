#pragma once
#include <string>
#include <vector>
#include <array>

#include "../BW/Constants.h"

namespace BW
{
  class dialog;
}

namespace BWAPI4
{
  class AutoMenuManager
  {
  public:
    AutoMenuManager();

    void reloadConfig();
    void chooseNewRandomMap();
    void onMenuFrame();

    // cppcheck-suppress functionConst
    const char* interceptFindFirstFile(const char *lpFileName);

    std::string autoMenuSaveReplay;
  private:
    static void pressDialogKey(BW::dialog *pDlg);

    std::string autoMenuMode;
    std::string autoMenuCharacterName;
    std::string autoMenuLanMode;
    std::string autoMenuRestartGame;
    std::string autoMenuGameType;
    std::string autoMenuGameTypeExtra;
    std::string autoMenuGameName;
    unsigned lastAutoMapEntry = 0;
    std::string lastMapGen;
    std::vector<std::string> autoMapPool;
    std::string autoMenuMapPath;
    std::string autoMapIteration = "RANDOM";

    std::string autoMenuRace;
    std::array<std::string, BW::PLAYABLE_PLAYER_COUNT> autoMenuEnemyRace;
    unsigned autoMenuEnemyCount = 0;
    unsigned autoMenuMinPlayerCount = 0;
    unsigned autoMenuMaxPlayerCount = 0;
    unsigned autoMenuWaitPlayerTime = 0;

    unsigned autoMapTryCount = 0;

    bool actStartedGame = false;
    bool actRaceSel = false;

#ifdef _DEBUG
    std::string autoMenuPause;
#endif
  };




}
