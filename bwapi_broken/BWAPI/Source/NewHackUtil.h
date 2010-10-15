/*  HackUtil is a set of functions that make import detours and code patching easier.
    Copyright (C) 2010  Adam Heinermann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string>
#include <windows.h>
#include <vector>

// Typedefs
typedef unsigned __int8  u8 ;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef signed   __int8  s8 ;
typedef signed   __int16 s16;
typedef signed   __int32 s32;
typedef signed   __int64 s64;

// Variables
#define HU_BUFFER_SIZE 1024

#define __TOSTRING(l) #l
#define _TOSTRING(l) __TOSTRING(l)
#define FILELINE __FILE__ ":" _TOSTRING(__LINE__)

namespace HackUtil
{
  // Imports
  IMAGE_IMPORT_DESCRIPTOR* _GetImportDescriptor(HMODULE module);
  IMAGE_THUNK_DATA32*      _GetImportsList(char* sourceModule, char* importModule);
  DWORD*                   _GetFunctionsList(char* sourceModule, char* importModule);
  /* These functions are not specifically made for public use */

  bool PatchImport(char* sourceModule, char* importModule, LPCSTR name, void* patchFunction);
  bool PatchImport(char* importModule, LPCSTR name, void* patchFunction);
  bool PatchImport(char* sourceModule, char* importModule, int ordinal, void* patchFunction);
  bool PatchImport(char* importModule, int ordinal, void* patchFunction);
  /* Creates a detour for the specified import function in any loaded module */

  FARPROC GetImport(char* importModule, LPCSTR name);
  FARPROC GetImport(char* importModule, int ordinal);
  /* Retrieves the address of the imported function from the specified module */

  // Memory
  void WriteNops(void* dest, u32 size);
  void WriteNops(u32 dest, u32 size);
  /* Writes NOPs to the specified destination */

  void WriteMem(void* dest, void* source, u32 size);
  void WriteMem(u32 dest, void* source, u32 size);
  /* Writes data to the specified destination from the source */

  void JmpPatch(void* dest, void* patch);
  void JmpPatch(u32 dest, void* patch);
  /* Writes a jump to the specified patch at the destination */

  void CallPatch(void* dest, void* patch);
  void CallPatch(u32 dest, void* patch);
  /* Writes a call to the specified patch at the destination */

  void Revert(const char *logline);
  /* Reverts all changes made with any function except for WriteMemRaw */

  void WriteMemRaw(void* dest, void* source, u32 size);
  void WriteMemRaw(u32 dest, void* source, u32 size);
  /* Writes to memory and does not include the change in the changes list */
};
