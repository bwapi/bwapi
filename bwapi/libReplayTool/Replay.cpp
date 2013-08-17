#include "Replay.h"
#include <fstream>
#include <list>
#include "PKShared.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "ReplayReader.h"
#include "RepHeader.h"
#include "GameAction.h"
#include "ActionParser.h"
#include "ParseReplayParams.h"

using namespace std;
using namespace ReplayTool;

replay_resource replayHeader;

void writeBuffer(const char *pszFormat, const char *pszFilename, void *pBuffer, DWORD dwBufferSize)
{
  FileWriter fw;
  char szTmp[MAX_PATH];
  sprintf(szTmp, pszFormat, pszFilename);
  if ( fw.Open( szTmp ) )
    fw.WriteRaw(pBuffer, dwBufferSize);
}

bool errSimple(const char *pszText)
{
  std::ofstream log("Replay_errLog.log", std::ios_base::app);
  log << pszText << "\n";
  log.close();
  return false;
}

/*!
 * @fn void getActionsTraceFilepath(const ParseReplayParams& params, char actionsDbgFilepath[MAX_PATH])
 * @brief Generates a unique filename for the action trace file for a given replay.
 * The action trace filename format: <replay-filename>.[<ID>.]trace.txt
 * where ID is a unique number > 0 that is used if file with name <replay-filename>.trace.txt already exists
 * Example: if the provided replay filename is LastReplay.rep, then the generated action trace filename is LastReplay.trace.txt
 * and if LastReplay.trace.txt already exists, then the generated one is LastReplay.1.trace.txt, and so on ...
 * @param actionsDbgFilepath[out] a complete filepath of the action trace file.
 */
void getActionsTraceFilepath(const ParseReplayParams& params, char actionsDbgFilepath[MAX_PATH]) 
{
  fstream actionsDbgFile;
  const char* replayFilename = strrchr(params.getReplayPath(), '\\');

  // In case the path has no slashes i.e the replay file name only
  if (NULL == replayFilename)
  {
    replayFilename = params.getReplayPath();
  }
  else
  {
    // stand directly after the slash
    ++replayFilename;
  }

  actionsDbgFilepath[0] = '\0';

  sprintf_s(actionsDbgFilepath, MAX_PATH, "%s\\%s.trace.txt", params.getOutRepoPath(), replayFilename);
  actionsDbgFile.open(actionsDbgFilepath, ios::in | ios::out | ios::_Nocreate);

  // File could be opened successfully which means that a filename collision happened
  // a file with the same name already exists, need to generate a unique filename
  if (actionsDbgFile)
  {
    unsigned nameCollisionID = 0;

    do
    {
      actionsDbgFile.close();

      actionsDbgFilepath[0] = '\0';
      ++nameCollisionID;
      sprintf_s(actionsDbgFilepath, MAX_PATH, "%s\\%s.%u.trace.txt", params.getOutRepoPath(), replayFilename, nameCollisionID);

      actionsDbgFile.open(actionsDbgFilepath, ios::in | ios::out | ios::_Nocreate);
    }
    while (actionsDbgFile);
  }

  actionsDbgFile.close();
}

extern "C" bool parseReplay(const ParseReplayParams& params, DWORD dwFlags)
{
  // Open replay file
  FileReader fr;
  if ( !fr.Open(params.getReplayPath()) )
    return false;

///////////////////// Header
  // Read replay resource identifier
  DWORD dwRepResourceID = 0;
  // Best guess: "reRS" is "replay RESOURCE"
  if ( !DecompressRead(&dwRepResourceID, sizeof(dwRepResourceID), fr) || dwRepResourceID != mmioFOURCC('r','e','R','S') )
    return errSimple("No Replay resource ID found.");

  // Read replay resource header
  if ( !DecompressRead(&replayHeader, sizeof(replayHeader), fr) )
    return errSimple("Unable to read replay header.");

////////////////// Actions
  // Read replay actions section size
  DWORD dwActionBufferSize = 0;
  if ( !DecompressRead(&dwActionBufferSize, 4, fr) )
    return errSimple("Unable to read actions size.");

  // Allocate and Read replay actions
  ReplayReader repActions(dwActionBufferSize);
  if ( dwActionBufferSize && (!repActions || !DecompressRead(repActions, dwActionBufferSize, fr)) )
    return errSimple("Decompressing actions failed.");

/////////////////// Map Chk
  // get map chunk data size
  DWORD dwChkBufferSize = 0;
  if ( !DecompressRead(&dwChkBufferSize, 4, fr) )
    return errSimple("Unable to read chk size.");

  // Allocate and Read chk data
  void *pChkBuffer = malloc(dwChkBufferSize);
  //FileReader frChk(pChkBuffer, dwChkBufferSize);
  if ( dwChkBufferSize && !DecompressRead(pChkBuffer, dwChkBufferSize, fr) )
    return errSimple("Decompressing map failed.");

  // Write extracted replay data
  if ( dwFlags & RFLAG_EXTRACT )
  {
    writeBuffer("%s.hdr", params.getReplayPath(), &replayHeader, sizeof(replayHeader));
    writeBuffer("%s.act", params.getReplayPath(), repActions, dwActionBufferSize);
    writeBuffer("%s.chk", params.getReplayPath(), pChkBuffer, dwChkBufferSize);
  }

  // parse data for repair
  if ( dwFlags & RFLAG_REPAIR )
  {
    list<ReplayTool::GameAction*> actions;
    char actionsDbgFilepath[MAX_PATH];

    getActionsTraceFilepath(params, actionsDbgFilepath);

    // Parse replay actions
    parseActions(repActions, actions);
    logActions(actions, actionsDbgFilepath);

    if ( replayHeader.dwFrameCount < repActions.highestFrameTick() )
    {
      std::ofstream log("Results.txt", std::ios_base::app);
      log << params.getReplayPath() << " -- Fixed replay with " << replayHeader.dwFrameCount << " frames. Desired: " << repActions.highestFrameTick() << " frames.\n";

      replayHeader.dwFrameCount = repActions.highestFrameTick() + 100;

      // Repair/reconstruct the replay
      FileWriter fw;
      fw.Open(params.getReplayPath());

      // write rep resource id
      dwRepResourceID = mmioFOURCC('r','e','R','S');
      CompressWrite(&dwRepResourceID, sizeof(dwRepResourceID), fw);

      // write header
      CompressWrite(&replayHeader, sizeof(replayHeader), fw);

      // write actions
      CompressWrite(&dwActionBufferSize, sizeof(dwActionBufferSize), fw);
      if ( dwActionBufferSize )
        CompressWrite(repActions, repActions.size(), fw);
    
      // write chk
      CompressWrite(&dwChkBufferSize, sizeof(dwChkBufferSize), fw);
      if ( dwChkBufferSize )
        CompressWrite(pChkBuffer, dwChkBufferSize, fw);

      log.close();
    } // if replay is damaged
  }

  return true;
}

