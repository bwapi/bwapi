#pragma once

/**
 *  An array of entries with busy flags. Can be used:
 *  - as a set with integer keys below SIZE
 *  - as a slot array allocator / chunk memory manager
 *
 */

#include "Types.h"

#include <stdexcept>

namespace Util
{
  template<typename T> class Indexed
  {
  public:
    Indexed(int index, T item)
      : index(index)
      , item(item)
    {
    }
    int index;
    T item;

    // allow the sweetest eyecandy
    // you can just specify int or T in foreach,
    // and successfully retrieve only that value
    operator int() const
    {
      return index;
    }

    operator T() const
    {
      return item;
    }
  };

  template<typename T, int SIZE>
  struct StaticSet
  {
  private:
    struct Slot
    {
      T item;
      bool valid;
    };
  public:
    typedef T ElementType;
    static const int size = SIZE;

    s32 count;
    // used to optimize allocations in large sets
    // all slots before head are valid, head itself may point to a valid or
    // an invalid slot
    s32 head;
    Slot data[SIZE];

    //---- make this an std compliant container ---------------------------
    class const_iterator
    {
      friend struct StaticSet;
    private:
      const_iterator(const Slot *data, int index, int validIndex, int validCount)
        : data(data)
        , index(index)
        , validIndex(validIndex)
        , validCount(validCount)
      {}

      int validIndex;
      int validCount;
      int index;
      const Slot *data;
    public:
      void operator++()
      {
        if(validIndex >= validCount)
          throw std::out_of_range("iterator ++'ed past range");
        do
        {
          index++;
        }while(!data[index].valid);
        validIndex++;
      }
      Indexed<const T&> operator*() const
      {
        return Indexed<const T&>(index, data[index].item);
      }
      bool operator==(const const_iterator &b) const
      {
        return validIndex == b.validIndex;
      }
      bool operator!=(const const_iterator &b) const
      {
        return !operator==(b);
      }
    };

    const_iterator begin() const
    {
      return const_iterator(data, 0, 0, count);
    }

    const_iterator end() const
    {
      return const_iterator(data, size, count, count);
    }

    //---- general functions ----------------------------------------
    void reset()
    {
      for(int i = 0;i < size; i++)
        data[i].valid = false;
      head = 0;
      count = 0;
    }

    bool inline isInRange(int index) const
    {
      return index >= 0 && index < (int)size;
    }

    //---- set functions --------------------------------------------
    const T& at(int index) const
    {
      if(!isInRange(index))
        throw std::out_of_range("index out of range");
      if(!data[index].valid)
        throw std::out_of_range("slot is not valid");
      return data[index].item;
    }

    T& at(int index)
    {
      if(!isInRange(index))
        throw std::out_of_range("index out of range");
      if(!data[index].valid)
        throw std::out_of_range("slot is not valid");
      return data[index].item;
    }

    // same as at, but without error checking
    const T& operator[](int index) const
    {
      return data[index].item;
    }

    T& operator[](int index)
    {
      return data[index].item;
    }

    void set(int index, const T& source)
    {
      if(!isInRange(index))
        throw std::out_of_range("index out of range");
      if(!data[index].valid)
      {
        count++;
        data[index].valid = true;
      }
      data[index].item = source;
    }

    T& allocate(int index)
    {
      if(!isInRange(index))
        throw std::out_of_range("index out of range");
      if(data[index].valid)
        throw std::bad_alloc("slot already occupied");
      if(!data[index].valid)
      {
        count++;
        data[index].valid = true;
      }
      return data[index].item;
    }

    void free(int index)
    {
      if(!isInRange(index))
        throw std::out_of_range("index out of range");
      if(data[index].valid)
      {
        count--;
        data[index].valid = false;
      }
      if(index < head)
        head = index;
    }

    bool isValid(int index) const
    {
      if(index < 0 || index >= (int)head;)
        return false;
      return data[index].valid;
    }

    //---- slot functions -------------------------------------------
    int findEmptySlot()
    {
      if(count == size)
        throw std::bad_alloc("StaticSet ran out of slots");
      for(;;)
      {
        if(head == size)
          throw std::logic_error("StaticSet count or head failure"); // either count or head have wrong values
        if(!data[head].valid)
          break;
        head++;
      }
      return head;
    }
  };
}
