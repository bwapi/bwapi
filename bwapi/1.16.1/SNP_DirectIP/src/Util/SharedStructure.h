#pragma once
////////////////////////////////////////////////////////////////////////
//  A memory buffer which can be shared with other processes          //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util
{
  template<typename T> class SharedStructure;
}

#include "MemoryFrame.h"
#include "RemoteProcess.h"
#include "SharedMemory.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "windows.h"

namespace Util
{
  template<typename T> class SharedStructure
  {
  public:
    //----------------------- TYPES -----------------------------------------------------
    template<typename S> struct Pointer
    {
      friend class SharedStructure;
    private:
      SharedMemory::Pointer<S> pointer;
    };
    struct Export
    {
      friend class SharedStructure;

      Export(){};
      bool isValid() const
      {
        return smemExport.isValid();
      }
    private:
      SharedMemory::Export smemExport;
    };
    //----------------------- CONSTRUCTION ----------------------------------------------
    SharedStructure()
      : smem()
    {
    }
    ~SharedStructure()
    {
    }
    //----------------------- CREATE ----------------------------------------------------
    void create()
    {
      smem.create(sizeof(T));
    }
    bool create(const char* systemName)
    {
      return smem.create(sizeof(T), systemName);
    }
    //----------------------- IMPORT ----------------------------------------------------
    void import(Export source)
    {
      smem.import(source.smemExport);
    }
    //----------------------- RELEASE ---------------------------------------------------
    void release()
    {
      smem.release();
    }
    //----------------------- EXPORT TO PROCESS -----------------------------------------
    Export exportToProcess(RemoteProcess &target, bool readOnly) const
    {
      Export exp;
      exp.smemExport = smem.exportToProcess(target, readOnly);
      return exp;
    }
    //----------------------- GET MEMORY ------------------------------------------------
    T &get() const
    {
      return smem.getMemory().getAs<T>();
    }
    //----------------------- -----------------------------------------------------------
  private:
    SharedStructure(SharedStructure&);                // no copying

    SharedMemory smem;
  };
}