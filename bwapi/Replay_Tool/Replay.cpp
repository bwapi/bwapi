#include "Replay.h"

#include "PKShared.h"

#include "FileReader.h"
#include "FileWriter.h"

#include "RepHeader.h"
#include "Actions.h"

replay_resource replayHeader;

void WriteBuffer(const char *pszFormat, const char *pszFilename, void *pBuffer, DWORD dwBufferSize)
{
  FileWriter fw;
  char szTmp[MAX_PATH];
  sprintf(szTmp, pszFormat, pszFilename);
  if ( fw.Open( szTmp ) )
    fw.WriteRaw(pBuffer, dwBufferSize);
}

bool errSimple(const char *pszText)
{
  MessageBox(NULL, pszText, NULL, 0);
  return false;
}

bool ParseReplay(const char *pszFilename, DWORD dwFlags)
{
  // Open replay file
  FileReader fr;
  if ( !fr.Open(pszFilename) )
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
  void *pActionBuffer = malloc(dwActionBufferSize);
  FileReader frActions(pActionBuffer, dwActionBufferSize);
  if ( dwActionBufferSize && !DecompressRead(pActionBuffer, dwActionBufferSize, fr) )
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
    WriteBuffer("%s.hdr", pszFilename, &replayHeader, sizeof(replayHeader));
    WriteBuffer("%s.act", pszFilename, pActionBuffer, dwActionBufferSize);
    WriteBuffer("%s.chk", pszFilename, pChkBuffer, dwChkBufferSize);
  }

  // parse data for repair
  if ( dwFlags & RFLAG_REPAIR )
  {
    // Parse replay actions
    ParseActions(frActions);

    if ( replayHeader.dwFrameCount < g_dwHighestFrame )
    {
      char szTmp[256];
      sprintf(szTmp, "Fixed replay with %u frames. Desired: %u frames.", replayHeader.dwFrameCount, g_dwHighestFrame);
      MessageBox(NULL, szTmp, "Fixed", 0);

      replayHeader.dwFrameCount = g_dwHighestFrame + 10;
    }

    FileWriter fw;
    fw.Open(pszFilename);

    // write rep resource id
    dwRepResourceID = mmioFOURCC('r','e','R','S');
    CompressWrite(&dwRepResourceID, sizeof(dwRepResourceID), fw);

    // write header
    CompressWrite(&replayHeader, sizeof(replayHeader), fw);

    // write actions
    CompressWrite(&dwActionBufferSize, sizeof(dwActionBufferSize), fw);
    if ( dwActionBufferSize )
      CompressWrite(pActionBuffer, dwActionBufferSize, fw);
    
    // write chk
    CompressWrite(&dwChkBufferSize, sizeof(dwChkBufferSize), fw);
    if ( dwChkBufferSize )
      CompressWrite(pChkBuffer, dwChkBufferSize, fw);
  }

  return true;
}

