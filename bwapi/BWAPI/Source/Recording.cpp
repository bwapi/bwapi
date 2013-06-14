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
void *pVidBuffer;

SIZE gVidSize;

bool StartVideoRecording(int width, int height)
{
  // Initialize AVI thingy
  AVIFileInit();

  // Stop recording if it is already recording
  if ( recordingStarted )
    StopVideoRecording();
  
  // Show the cursor so that the popup dialogs do not hide the cursor
  ShowCursor(TRUE);

  gVidSize.cx = width;
  gVidSize.cy = height;

  char szFileName[MAX_PATH] = { 0 };

  // Initialize OPENFILENAME struct
  OPENFILENAME ofn = { 0 };
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.lpstrFilter = "Video Files (*.avi)\0*.AVI\0\0\0";
  ofn.Flags       = OFN_DONTADDTORECENT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = "avi";
  ofn.lpstrFile   = szFileName;
  ofn.nMaxFile    = MAX_PATH;

  // Obtain the file name for the video
  if ( !GetSaveFileName(&ofn) )
  {
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  // Create the AVI file
  if ( AVIFileOpen(&pAviFile, szFileName, OF_WRITE | OF_CREATE, NULL) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIFileOpen failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  // Initialize new bitmap info header
  BITMAPINFOHEADER vidBmpInfo = wmodebmp.bmiHeader;
  vidBmpInfo.biBitCount       = 32;
  vidBmpInfo.biHeight         = abs(vidBmpInfo.biHeight);
  vidBmpInfo.biSizeImage      = vidBmpInfo.biWidth*vidBmpInfo.biHeight*4;
  vidBmpInfo.biXPelsPerMeter  = 10000;
  vidBmpInfo.biYPelsPerMeter  = 10000;
  vidBmpInfo.biClrUsed        = 0;
  vidBmpInfo.biClrImportant   = 0;

  // initialize avistream info structure
  AVISTREAMINFO aisinfo = { 0 };
  aisinfo.fccType               = streamtypeVIDEO;
  aisinfo.dwScale               = 42;
  aisinfo.dwRate                = 1000;
  aisinfo.dwSuggestedBufferSize = width * height * 4;
  aisinfo.dwQuality             = (DWORD)(-1);
  aisinfo.dwSampleSize          = width * height * 4;
  SetRect(&aisinfo.rcFrame, 0, 0, width, height);

  // Create the uncompressed stream
  if ( AVIFileCreateStream(pAviFile, &pAviStream, &aisinfo) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIFileCreateStream failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  // Retrieve the AVI compression options via popup dialog
  AVICOMPRESSOPTIONS *pOptions = &aviOptions;
  if ( !AVISaveOptions(NULL, 0, 1, &pAviStream, &pOptions) )
  {
    MessageBox(NULL, "AVISaveOptions failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  // Create the compressed AVI stream
  if ( AVIMakeCompressedStream(&pAviStreamCompressed, pAviStream, pOptions, NULL) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIMakeCompressedStream failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }
  
  // Set the compressed stream format
  if ( AVIStreamSetFormat(pAviStreamCompressed, 0, &vidBmpInfo, sizeof(BITMAPINFOHEADER)) != AVIERR_OK )
  {
    MessageBox(NULL, "AVIStreamSetFormat failed!", "Recording failed!", MB_OK | MB_ICONHAND);
    ShowCursor(FALSE);
    return StopVideoRecording();
  }

  // Allocate memory for temp
  pVidBuffer = malloc(width*height);

  // Allocate memory for flipped video surface
  pFlipped = malloc(width*height*4);

  // Hide the cursor again
  ShowCursor(FALSE);
  recordingStarted = true;

  return true;
}

bool StopVideoRecording()
{
  // De-allocate and close handles
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

  if ( pVidBuffer )
    free(pVidBuffer);
  pVidBuffer = NULL;

  AVIFileExit();
  return false;
}

void RecordFrame(void *pBuffer)
{
  if ( !pBuffer )
    return;

  DWORD *pbFlipped = (DWORD*)((DWORD)pFlipped + gVidSize.cx*(gVidSize.cy-1)*4);
  BYTE  *pbSrc     = (BYTE*)pBuffer;
  DWORD dwEnd      = (DWORD)pBuffer + gVidSize.cx * gVidSize.cy;
  do
  {
    for ( unsigned int i = 0; i < (unsigned int)gVidSize.cx; ++i )
      pbFlipped[i] = (DWORD&)wmodebmp.bmiColors[pbSrc[i]];
    pbFlipped -= gVidSize.cx;
    pbSrc += gVidSize.cx;
  } while ( (DWORD)pbSrc < dwEnd );
  AVIStreamWrite(pAviStreamCompressed, dwFrames++, 1, pFlipped, gVidSize.cx*gVidSize.cy*4, AVIIF_KEYFRAME, NULL, NULL);
}
