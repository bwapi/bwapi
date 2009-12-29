#pragma once
/**
 *  A map stored as an array for constant time access.
 */

#include "Strings.h"

#include <vector>

namespace Util
{
  template<typename T> class LookupTable
  {
  private:
    T defaultValue;
    bool isDefaultValueSpecified;

    struct Entry
    {
      bool valid;
      T value;
    };
    Entry invalidEntry;
    std::vector<Entry> data;
  public:
    LookupTable()
      : isDefaultValueSpecified(false)
    {
      invalidEntry.valid = false;
    }
    ~LookupTable()
    {
    }

    void setDefaultValue(const T& defaultValue)
    {
      this->defaultValue = defaultValue;
      isDefaultValueSpecified = true;
    }
    void setValue(int index, const T& value)
    {
      Entry entry;
      entry.valid = true;
      entry.value = value;
      data.resize(max(data.size(), index+1), invalidEntry);
      data.at(index) = entry;
    }
    /**
     *  The default value is returned on retrieval of a non existent element.
     *  An exception is thrown if no default value is specified
     */
    const T &lookUp(int index) const
    {
      if(index >= 0 && index < data.size())
      {
        const Entry& entry = data.at(index);
        if(entry.valid)
        {
          return entry.value;
        }
      }
      if(!isDefaultValueSpecified)
        throw GeneralException("LookupTable does not hold value " + Strings::intToString(index));
      return defaultValue;
    }
  };
};
