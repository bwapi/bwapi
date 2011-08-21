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

bool StartVideoRecording(const char *pszFilename, int width, int height)
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

  BITMAPINFOHEADER vidBmpInfo = bmp.bmiHeader;
  vidBmpInfo.biBitCount       = 32;
  vidBmpInfo.biHeight         = abs(vidBmpInfo.biHeight);
  vidBmpInfo.biSizeImage      = vidBmpInfo.biWidth*vidBmpInfo.biHeight*4;
  vidBmpInfo.biXPelsPerMeter  = 10000;
  vidBmpInfo.biYPelsPerMeter  = 10000;
  vidBmpInfo.biClrUsed        = 0;
  vidBmpInfo.biClrImportant   = 0;

  AVISTREAMINFO aisinfo = { 0 };
  aisinfo.fccType               = streamtypeVIDEO;
  aisinfo.dwScale               = 1;
  aisinfo.dwRate                = 30;
  aisinfo.dwSuggestedBufferSize = width * height * 4;
  aisinfo.dwQuality             = (DWORD)(-1);
  aisinfo.dwSampleSize          = width * height * 4;
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
  
  if ( pAviStreamCompressed->SetFormat(0, &vidBmpInfo, sizeof(BITMAPINFO256)) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIStreamSetFormat failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }
  // Allocate memory for flipped video surface
  pFlipped = malloc(width*height*4);

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
  DWORD *pbFlipped = (DWORD*)((DWORD)pFlipped + width*(height-1)*4);
  BYTE  *pbSrc     = (BYTE*)pBuffer;
  DWORD dwEnd      = (DWORD)pBuffer + width*height;
  do
  {
    for ( unsigned int i = 0; i < (unsigned int)width; ++i )
      pbFlipped[i] = *(DWORD*)&bmp.bmiColors[pbSrc[i]];
    pbFlipped -= width;
    pbSrc += width;
  } while ( (DWORD)pbSrc < dwEnd );
  pAviStreamCompressed->Write(dwFrames, 1, pFlipped, width*height*4, dwFrames % 32 == 0 ? AVIIF_KEYFRAME : 0, NULL, NULL);
  ++dwFrames;
}
