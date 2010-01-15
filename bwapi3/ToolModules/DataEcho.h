#pragma once

/**
 *  This class is a holder for gamestate from previous frame. It also
 *  provides convenience functions to compare current state with the previous
 */

namespace BWAPI
{
  template<typename T>  // T is the datatype which is to store
  class DataEcho
  {
  private:
    T data;
    T* original
  public:

    // retrieves saved data.
    T& getData()
    {
      return this->data;
    }

    // copies the data from the original source
    // also stores the pointer to the original data so
    // comparisons can be directly made with that data
    void fetch(T* original)
    {
      this->original = original;
      memcpy(&data, original, sizeof(T));
    }

    // compares the element from original data you pass with
    // the local copy of it
    template<typename D>
    bool isChanged(D* originalVariable)
    {
      D* storedVariable = (int)originalVariable - (int)original + (int)&data;
      bool isSame = memcmp(originalVariable, storedVariable, sizeof(D)) == 0;
      return !isSame;
    }
  };
}
