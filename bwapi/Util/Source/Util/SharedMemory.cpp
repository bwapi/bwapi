#include "SharedMemory.h"

namespace Util
{
  //----------------------- CONSTRUCTION ------------------------------
  SharedMemory::SharedMemory()
    : mappingObjectHandle(INVALID_HANDLE_VALUE)
    , bufferBase(NULL)
    , bufferSize(0)
  {
  }

  SharedMemory::~SharedMemory()
  {
    release();
  }

  //----------------------- CREATE -------------------------------------
  bool SharedMemory::create(int size, const char* systemName)
  {
    this->mappingObjectHandle = ::CreateFileMappingA(
      INVALID_HANDLE_VALUE,   // use page file for backup
      NULL,                   // not inheritable
      PAGE_READWRITE,         // access rights
      0,                      // Hi size
      size,                   // Lo size
      systemName);            // Unnamed
    if(this->mappingObjectHandle == NULL)
      throw GeneralException(__FUNCTION__ ": CreateFileMapping failed");
    bool alreadyExisted = GetLastError() == ERROR_ALREADY_EXISTS;

    this->bufferBase = ::MapViewOfFile(
      this->mappingObjectHandle,
      FILE_MAP_ALL_ACCESS,    // access rights
      0,                      // Hi offset
      0,                      // Lo offset
      size);                  // size
    if(this->bufferBase == NULL)
    {
      CloseHandle(this->mappingObjectHandle);
      throw GeneralException(__FUNCTION__ ": MapViewOfFile failed");
    }

    this->bufferSize = size;

    return !alreadyExisted;
  }
  //----------------------- IMPORT -------------------------------------
  void SharedMemory::import(Export source)
  {
    this->bufferBase = ::MapViewOfFile(
      source.targetProcessMappingObjectHandle,
      source.readOnly ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS,
      0,                      // Hi offset
      0,                      // Lo offset
      source.bufferSize);     // size
    if(this->bufferBase == NULL)
      throw GeneralException(__FUNCTION__ ": failed MapViewOfFile");

    this->bufferSize = source.bufferSize;
  }
  //----------------------- BOUNDARY -----------------------------------
  MemoryFrame SharedMemory::getMemory() const
  {
    return MemoryFrame(this->bufferBase, this->bufferSize);
  }
  //----------------------- EXPORT TO PROCESS --------------------------
  SharedMemory::Export SharedMemory::exportToProcess(RemoteProcess &target, bool readOnly) const
  {
    HANDLE targetProcessHandle = target.getHandle();
    HANDLE exportHandle;
    BOOL success = DuplicateHandle(
      GetCurrentProcess(),        // source process handle
      this->mappingObjectHandle,  // source handle
      targetProcessHandle,        // target process handle
      &exportHandle,              // out: target handle
      NULL,                       // access
      false,                      // inherit
      readOnly ? PAGE_READONLY : DUPLICATE_SAME_ACCESS);
    if(!success)
    {
      throw GeneralException(__FUNCTION__ ": failed export SharedMemory");
    }
    Export exportObject;
    exportObject.bufferSize = this->bufferSize;
    exportObject.targetProcessMappingObjectHandle = exportHandle;
    exportObject.readOnly = readOnly;
    return exportObject;
  }
  //----------------------- RELEASE ------------------------------------
  void SharedMemory::release()
  {
    if(this->mappingObjectHandle != INVALID_HANDLE_VALUE)
    {
      CloseHandle(this->mappingObjectHandle);
    }
    if(this->bufferBase)
    {
      UnmapViewOfFile(this->bufferBase);
    }
    this->bufferBase = NULL;
    this->bufferSize = 0;
    this->mappingObjectHandle = INVALID_HANDLE_VALUE;
  }
  //----------------------- PAGE SIZE ----------------------------------
  int SharedMemory::getPageSize()
  {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwPageSize;
  }
  //--------------------------------------------------------------------
  //----------------------- CONSTRUCTION -------------------------------
  SharedMemory::Export::Export()
    : targetProcessMappingObjectHandle(INVALID_HANDLE_VALUE)
  ,bufferSize(0)
  ,readOnly(true)
  {
  }
  //----------------------- --------------------------------------------
}
