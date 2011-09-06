/*****************************************************************************/
/* pklib.h                                Copyright (c) Ladislav Zezula 2003 */
/*---------------------------------------------------------------------------*/
/* Header file for PKWARE Data Compression Library                           */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 31.03.03  1.00  Lad  The first version of pkware.h                        */
/*****************************************************************************/

#ifndef __PKLIB_H__
#define __PKLIB_H__

//-----------------------------------------------------------------------------
// Defines

#define CMP_BUFFER_SIZE    36312        // Size of compression buffer
#define EXP_BUFFER_SIZE    12596        // Size of decompress buffer

#define CMP_BINARY             0        // Binary compression
#define CMP_ASCII              1        // Ascii compression

#define CMP_NO_ERROR           0
#define CMP_INVALID_DICTSIZE   1
#define CMP_INVALID_MODE       2
#define CMP_BAD_DATA           3
#define CMP_ABORT              4

//-----------------------------------------------------------------------------
// Define calling convention

//#define PKEXPORT
#define PKEXPORT  __cdecl               // Use for normal __cdecl calling 
//#define PKEXPORT  __stdcall
//#define PKEXPORT  __fastcall

//-----------------------------------------------------------------------------
// Public functions

#ifdef __cplusplus
   extern "C" {
#endif

unsigned int PKEXPORT implode(
   unsigned int (PKEXPORT* read_buf)(char *buf, unsigned int *size, void *param),
   void         (PKEXPORT* write_buf)(char *buf, unsigned int *size, void *param),
   char         *work_buf,
   void         *param,
   unsigned int *type,
   unsigned int *dsize);


unsigned int PKEXPORT explode(
   unsigned int (PKEXPORT* read_buf)(char *buf, unsigned  int *size, void *param),
   void         (PKEXPORT* write_buf)(char *buf, unsigned  int *size, void *param),
   char         *work_buf,
   void         *param);

// The original name "crc32" was changed to "crc32pk" due
// to compatibility with zlib
unsigned long PKEXPORT crc32pk(char *buffer, unsigned int *size, unsigned long *old_crc);

#ifdef __cplusplus
   }                         // End of 'extern "C"' declaration
#endif

#endif // __PKLIB_H__
