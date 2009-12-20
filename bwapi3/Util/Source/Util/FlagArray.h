#pragma once
/**
 *  An array of bools with additional optimized block search feature
 */

#include "Buffer.h"

namespace Util
{
  class FlagArray
  {
  private:
    Buffer buffer;
    int* words;
    int flagCount;
  public:
    FlagArray();
    ~FlagArray();
    void setSize(int flagCount);
    int size();   // returns flag count
    bool getFlag(int flagIndex);
    void setFlag(int flagIndex, bool flagState);
    int getWordCount();
    int getWord(int wordIndex);
    int findNextWord(int startIndex); // does return startIndex if it's not null
    void setAllFlags(bool flagState);

    static const int flagsInWord = sizeof(int) * 8;
  };
}
