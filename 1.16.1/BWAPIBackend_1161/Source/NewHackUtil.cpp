#include "NewHackUtil.h"

namespace HackUtil
{
  struct memPatch
  {
    void* location;
    char  patch[128];
    u32   patchSize;
  };

  static std::vector<memPatch> changes;
  ///////////////////////////////   IMPORTS   //////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  IMAGE_IMPORT_DESCRIPTOR* _GetImportDescriptor(HMODULE module)
  {
    IMAGE_DOS_HEADER *mzhead = (IMAGE_DOS_HEADER*)module;
    if (mzhead->e_magic != IMAGE_DOS_SIGNATURE)
      return NULL;

    IMAGE_NT_HEADERS32 *pehead = (IMAGE_NT_HEADERS32*)(mzhead->e_lfanew + (u32)module);
    if (pehead->Signature != IMAGE_NT_SIGNATURE)
      return NULL;
      
    IMAGE_IMPORT_DESCRIPTOR *imports = (IMAGE_IMPORT_DESCRIPTOR*)(pehead->OptionalHeader.DataDirectory[1].VirtualAddress + (u32)module);
    return imports;
  }

  IMAGE_THUNK_DATA32* _GetImportsList(const char* sourceModule, const char* importModule)
  {
    HMODULE tempModule;
    tempModule = GetModuleHandleA(sourceModule);

    if ( !tempModule )
      return NULL;

    IMAGE_IMPORT_DESCRIPTOR *imports = _GetImportDescriptor(tempModule);
    if ( !imports )
      return NULL;
    
    for (u32 i = 0; imports[i].Name != 0; i++)
    {
      if (lstrcmpiA((char*)(imports[i].Name + (u32)tempModule), importModule) == 0)
        return (IMAGE_THUNK_DATA32*)(imports[i].OriginalFirstThunk + (u32)tempModule);
    }
    return NULL;
  }

  DWORD* _GetFunctionsList(const char* sourceModule, const char* importModule)
  {
    HMODULE tempModule = GetModuleHandleA(sourceModule);
    if ( !tempModule )
      return NULL;

    IMAGE_IMPORT_DESCRIPTOR *imports = _GetImportDescriptor(tempModule);
    if ( !imports )
      return NULL;

    for (u32 i = 0; imports[i].Name != 0; i++)
    {
      if (lstrcmpiA((char*)(imports[i].Name + (u32)tempModule), importModule) == 0)
        return (DWORD*)(imports[i].FirstThunk + (u32)tempModule);
    }
    return NULL;
  }

  FARPROC PatchImportOld(const char* sourceModule, const char* importModule, LPCSTR name, void* patchFunction)
  {
    if ( !name )
      return NULL;

    HMODULE tempModule = GetModuleHandleA(sourceModule);
    if ( !tempModule )
      return NULL;

    IMAGE_THUNK_DATA32* importOrigin = _GetImportsList(sourceModule, importModule);
    if ( !importOrigin )
      return NULL;

    DWORD* importFunction = _GetFunctionsList(sourceModule, importModule);
    if ( !importFunction )
      return NULL;

    for (u32 i = 0; importOrigin[i].u1.Ordinal != 0; i++)
    {
      if ((DWORD)name < 0xFFFF)
      {
        if (IMAGE_SNAP_BY_ORDINAL32(importOrigin[i].u1.Ordinal) && IMAGE_ORDINAL32(importOrigin[i].u1.Ordinal) == IMAGE_ORDINAL32((DWORD)name))
        {
          FARPROC oldFxn = (FARPROC)importFunction[i];
          WriteMem(&importFunction[i], &patchFunction, 4);
          return oldFxn;
        }
      }
      else
      {
#pragma warning(suppress: 6387)
        if (_strcmpi(name, (const char*)((PIMAGE_IMPORT_BY_NAME)((u32)importOrigin[i].u1.AddressOfData + (u32)tempModule))->Name) == 0)
        {
          FARPROC oldFxn = (FARPROC)importFunction[i];
          WriteMem(&importFunction[i], &patchFunction, 4);
          return oldFxn;
        }
      }
    }
    return NULL;
  }

  FARPROC GetImport(const char* importModule, LPCSTR name)
  {
    HMODULE module = GetModuleHandleA(importModule);
    if ( !module )
      return NULL;

    FARPROC rval = GetProcAddress(module, name);
    return rval;
  }
  
  FARPROC GetImport(const char *importModule, int ordinal)
  {
    return GetImport(importModule, (LPCSTR)ordinal);
  }

  ///////////////////////////////   MEMORY   //////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  void WriteNops(void* dest, u32 size)
  {
    memPatch tempPatch;
    tempPatch.location = dest;
    tempPatch.patchSize = size;
    memcpy_s(tempPatch.patch, 128, dest, size);
    changes.push_back(tempPatch);

    DWORD lastAccess = 0;
    VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &lastAccess);
    __try
    {
      memset(dest, 0x90, size);
    } 
    __except(1)
    {}
    VirtualProtect(dest, size, lastAccess, &lastAccess);
    return;
  }

  void WriteNops(u32 dest, u32 size)
  {
    WriteNops((void*)dest, size);
    return;
  }

  void WriteMem(void* dest, void* source, u32 size)
  {
    memPatch tempPatch;
    tempPatch.location = dest;
    tempPatch.patchSize = size;
    memcpy_s(tempPatch.patch, 128, dest, size);
    changes.push_back(tempPatch);

    WriteMemRaw(dest, source, size);
    return;
  }

  void WriteMemRaw(void* dest, void* source, u32 size)
  {
    DWORD lastAccess = 0;
    VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &lastAccess);
    __try
    {
      memcpy_s(dest, size, source, size);
    } 
    __except(1)
    {}
    VirtualProtect(dest, size, lastAccess, &lastAccess);
    return;
  }

  void WriteMem(u32 dest, void* source, u32 size)
  {
    WriteMem((void*)dest, source, size);
    return;
  }

  void WriteMemRaw(u32 dest, void* source, u32 size)
  {
    WriteMemRaw((void*)dest, source, size);
    return;
  }

  void JmpPatch(void* dest, void* patch)
  {
    u8 temp[5];
    temp[0] = 0xE9;
    *(int*)&temp[1] = (int)patch - (int)dest - 5;
    WriteMem(dest, temp, 5);
    return;
  }

  void JmpPatch(u32 dest, void* patch)
  {
    JmpPatch((void*)dest, patch);
    return;
  }

  void CallPatch(void* dest, void* patch)
  {
    u8 temp[5];
    temp[0] = 0xE8;
    *(int*)&temp[1] = (int)patch - (int)dest - 5;
    WriteMem(dest, temp, 5);
    return;
  }

  void CallPatch(u32 dest, void* patch)
  {
    CallPatch((void*)dest, patch);
    return;
  }

  void Revert(const char *logline)
  {
    for ( std::vector<memPatch>::iterator i = changes.begin(), iend = changes.end(); i != iend; ++i )
      WriteMemRaw(i->location, i->patch, i->patchSize);
    changes.clear();
    return;
  }
  
};
