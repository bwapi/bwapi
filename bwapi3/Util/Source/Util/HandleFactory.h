/**
 * Used by a native interface to allocate Handle state holding objects
 */

#include <list>

namespace Util
{
  template<typename T>
  class HandleFactory
  {
  private:
    //----------------------------- ----------------------------------------------
    struct Entry : T
    {
      bool busy;
    };
    static const int sizeOfT = sizeof(T); // doesn't work yet: ((sizeof(Entry)+sizeof(int)-1)/sizeof(int))*sizeof(int); // round up to next int boundary
    typedef std::list<Entry> EntrySet;
    EntrySet entrySet;
    //----------------------------- FIND -----------------------------------------
    typename EntrySet::iterator find(T* structure)
    {
      EntrySet::iterator it;
      for(it = entrySet.begin(); it != entrySet.end(); it++)
      {
        if(&(*it) == structure)
          break;
      }
      return it;
    }
    //----------------------------- ----------------------------------------------
  public:
    //----------------------------- CONSTRUCTION ---------------------------------
    HandleFactory()
    {
    }
    //----------------------------- CREATE ---------------------------------------
    T* create()
    {
      Entry newEntry;
      EntrySet::iterator it = entrySet.insert(entrySet.begin(), newEntry);
      return &(*it);
    }
    //----------------------------- IS VALID -------------------------------------
    bool isValid(T* structure)
    {
      return find(structure) == entrySet.end();
    }
    //----------------------------- RELEASE --------------------------------------
    void release(T* structure)
    {
      EntrySet::iterator it = find(structure);
      if(it == entrySet.end())
      {
        // usually the library user program screwed up
        return;
      }

      // remove structure
      entrySet.erase(it);

      return;
    }
    //----------------------------- RELEASE ALL ----------------------------------
    void releaseAll()
    {
      // free all items
      entrySet.clear();
    }
    //----------------------------- ----------------------------------------------
  };
}
