#include "Recording.h"
#include "WMode.h"

#include "BW/Offsets.h"

#include <vfw.h>
#include <windows.h>

bool recordingStarted;

PAVIFILE            pAviFile;
PAVISTREAM          pAviStream;
PAVISTREAM          pAviStreamCompressed;
AVICOMPRESSOPTIONS  aviOptions;

DWORD dwFrames;

void *pFlipped;

bool StartVideoRecording(const char *pszFilename, int width, int height, BITMAPINFO256 *bitmapInfo)
{
  AVIFileInit();

  if ( recordingStarted )
    StopVideoRecording();

  ShowCursor(TRUE);
  if ( AVIFileOpen(&pAviFile, pszFilename, OF_WRITE | OF_CREATE, NULL) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIFileOpen failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  AVISTREAMINFO aisinfo = { 0 };
  aisinfo.fccType               = streamtypeVIDEO;
  aisinfo.dwScale               = 1;
  aisinfo.dwRate                = 30;
  aisinfo.dwSuggestedBufferSize = width * height;
  aisinfo.dwQuality             = (DWORD)(-1);
  aisinfo.dwSampleSize          = width * height;
  SetRect(&aisinfo.rcFrame, 0, 0, width, height);

  if ( AVIFileCreateStream(pAviFile, &pAviStream, &aisinfo) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIFileCreateStream failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  AVICOMPRESSOPTIONS *pOptions = &aviOptions;
  if ( !AVISaveOptions(NULL, 0, 1, &pAviStream, &pOptions) )
  {
    MessageBox(NULL, "AVISaveOptions failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  if ( AVIMakeCompressedStream(&pAviStreamCompressed, pAviStream, pOptions, NULL) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIMakeCompressedStream failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }
  
  bitmapInfo->bmiHeader.biHeight = abs(bitmapInfo->bmiHeader.biHeight);
  if ( pAviStreamCompressed->SetFormat(0, bitmapInfo, sizeof(BITMAPINFO256)) != AVIERR_OK )
  {
    bitmapInfo->bmiHeader.biHeight = -bitmapInfo->bmiHeader.biHeight;
    MessageBox(NULL, "AVIStreamSetFormat failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }
  bitmapInfo->bmiHeader.biHeight = -bitmapInfo->bmiHeader.biHeight;

  pFlipped = malloc(width*height);

  ShowCursor(FALSE);
  recordingStarted = true;
  return true;
}

bool StopVideoRecording()
{
  recordingStarted = false;
  dwFrames         = 0;

  if ( pAviStream )
    AVIStreamClose(pAviStream);
  pAviStream = NULL;

  if ( pAviStreamCompressed )
    AVIStreamClose(pAviStreamCompressed);
  pAviStreamCompressed = NULL;

  if ( pAviFile )
    AVIFileClose(pAviFile);
  pAviFile = NULL;

  if ( pFlipped )
    free(pFlipped);
  pFlipped = NULL;

  AVIFileExit();
  return false;
}

void RecordFrame(void *pBuffer, int width, int height)
{
  BYTE *pbFlipped = (BYTE*)((DWORD)pFlipped + width*(height-1));
  BYTE *pbSrc     = (BYTE*)pBuffer;
  DWORD dwEnd     = (DWORD)pBuffer + width*height;
  while ( (DWORD)pbSrc < dwEnd )
  {
    memcpy(pbFlipped, pbSrc, width);
    pbFlipped -= width;
    pbSrc += width;
  }
  pAviStreamCompressed->Write(dwFrames, 1, pbFlipped, width*height, dwFrames % 16 == 0 ? AVIIF_KEYFRAME : 0, NULL, NULL);
  ++dwFrames;
}
