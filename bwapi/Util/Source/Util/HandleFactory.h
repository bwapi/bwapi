/**
 * Used by a native interface to allocate Handle state holding objects
 */

#include <set>

namespace Util
{
  template<typename T>
  class HandleFactory
  {
  private:
    //----------------------------- ----------------------------------------------
    struct Entry : T
    {
      int busy;
    };
    static const sizeOfT = (sizeof(Entry)+sizeof(int)-1)/sizeof(int); // round up to next int boundary
    Util::Buffer entryList;
    int count;
    int head;
    //----------------------------- ----------------------------------------------
  public:
    //----------------------------- CONSTRUCTION ---------------------------------
    HandleFactory()
      : count(0)
      , head(0)
    {
    }
    //----------------------------- CREATE ---------------------------------------
    T* create()
    {
      Entry *entry = entryList.getMemory().beginAs<Entry>();
      while(head < count)
      {
        Entry &curr = entry[head];
        if(!curr.busy)
          break;
        head++;
      }
      if(head == count)
      {
        // no new slot, allocate more memory
        count++;
        entryList.setSize(count * sizeOfT);
        Entry &newEntry = entryList.getMemory().endAs<Entry>()[-1];
        newEntry.busy = true;
        return &newEntry;
      }
      Entry &newEntry = entry[head];
      newEntry.busy = true;
      return &newEntry;
    }
    //----------------------------- IS VALID -------------------------------------
    bool isValid(T* structure)
    {
      Util::MemoryFrame mem = entryList.getMemory();
      if(&structure < mem.begin()
        || &structure > mem.end()
        || ((&structure - mem.begin()) % sizeOfT) != 0)
      {
        return false;
      }
      return true;
    }
    //----------------------------- RELEASE --------------------------------------
    void release(T* structure)
    {
      if(!isValid(structure))
      {
        // usually the library user program screwed up
        return;
      }

      // free slot
      Entry *entry = (Entry*)structure;
      entry->busy = false;

      // update head
      int index = (entry - mem.begin()) / sizeOfT;
      if(index < head)
        head = index;

      return;
    }
    //----------------------------- RELEASE ALL ----------------------------------
    void releaseAll()
    {
      // free each slot
      for(int i = 0; i < count; i++)
      {
        entryList[i].busy = false;
      }
    }
    //----------------------------- ----------------------------------------------
  }
}
