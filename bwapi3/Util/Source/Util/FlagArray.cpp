#include "FlagArray.h"

namespace Util
{
  //------------------------------- CONSTRUCTION ------------------------------------
  FlagArray::FlagArray()
    : flagCount(0)
  {
  }
  FlagArray::~FlagArray()
  {
  }
  //------------------------------- SET SIZE ----------------------------------------
  void FlagArray::setSize(int flagCount)
  {
    this->flagCount = flagCount;
    buffer.setSize(getWordCount() * sizeof(int));
    words = buffer.getMemory().beginAs<int>();
  }
  //------------------------------- SIZE --------------------------------------------
  int FlagArray::size()
  {
    return this->flagCount;
  }
  //------------------------------- GET FLAG ----------------------------------------
  bool FlagArray::getFlag(int flagIndex)
  {
    int mask = 1 << (flagIndex % flagsInWord);
    int masked = words[flagIndex / flagsInWord] & mask;
    return !!masked;
  }
  //------------------------------- SET FLAG ----------------------------------------
  void FlagArray::setFlag(int flagIndex, bool flagState)
  {
    int flagInQuestion = (1 << (flagIndex % flagsInWord));
    int &word = words[flagIndex / flagsInWord];

    int newWord = word;
    newWord &= ~flagInQuestion;
    if(flagState)
      newWord |= flagInQuestion;

    word = newWord;
  }
  //------------------------------- GET WORD COUNT ----------------------------------
  int FlagArray::getWordCount()
  {
    return (flagCount + flagsInWord - 1) / flagsInWord; // == round_up(flagCount/flagsInWord)
  }
  //------------------------------- GET WORD ----------------------------------------
  int FlagArray::getWord(int wordIndex)
  {
    return words[wordIndex];
  }
  //------------------------------- FIND NEXT WORD ----------------------------------
  int FlagArray::findNextWord(int startIndex)
  {
    int wordCount = getWordCount();
    for(;;)
    {
      if(startIndex >= wordCount)
        return -1;
      if(words[startIndex])
        return startIndex;
      startIndex++;
    }
  }
  //------------------------------- SET ALL FLAGS -----------------------------------
  void FlagArray::setAllFlags(bool flagState)
  {
    int newWord = flagState ? -1 : 0;
    int*end = words + getWordCount();
    for(int *it = words; it < end; it++)
    {
      *it = newWord;
    }
    // fill last word only with ones as far as flags go. e.g. 0b0001111111111111
    if(flagState)
    {
      // slit in two lines for debug purposes
      int lastWord = (((unsigned int)-1) >> (flagsInWord - (flagCount % flagsInWord)));
      words[getWordCount() - 1] = lastWord;
    }
  }
  //------------------------------- -------------------------------------------------
}
