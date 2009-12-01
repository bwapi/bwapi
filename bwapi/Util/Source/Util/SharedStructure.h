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
#define WIN32_LEAN_AND_MEAN
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
      SharedMemoryPointer<S> pointer;
    };
    class Export
    {
      friend class SharedStructure;
    public:
      Export(){};
      bool isValid() const
      {
        return smemExport.isValid();
      }
    private:
      SharedMemoryExport smemExport;
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
    bool create()
    {
      return smem.create(sizeof(T));
    }
    //----------------------- IMPORT ----------------------------------------------------
    bool import(Export source)
    {
      return smem.import(source.smemExport);
    }
    //----------------------- DISCARD ---------------------------------------------------
    void discard()
    {
      smem.discard();
    }
    //----------------------- EXPORT TO PROCESS -----------------------------------------
    Export exportToProcess(RemoteProcess &target, bool readOnly) const
    {
      Export exp;
      exp.smemExport = smem.exportToProcess(target, readOnly);
      return exp;
    }
    //----------------------- UNPACK ----------------------------------------------------
    template<typename S>
      S *unpack(Pointer<S> p) const // to process address space
      {
        return smem.unpack(p.pointer);
      }
    //----------------------- PACK ------------------------------------------------------
    template<typename S>
      Pointer<S> pack(S *pt) const  // to shared address space
      {
        Pointer<S> retval;
        retval.pointer = smem.pack(pt);
        return retval;
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