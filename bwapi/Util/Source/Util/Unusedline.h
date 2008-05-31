#pragma once

#include "DictionaryLine.h"

namespace Util
{
  /**
   * Represents line in property file that is not used to describe some property
   * value. The line but can have some other meaning like comment, section
   * delimiter or just empty line making the property file more easy to read.
   * @see Dictionary
   */
  class UnusedLine : public DictionaryLine
   {
    private :
      std::string line;
    public :
      UnusedLine(const std::string &line);
      std::string getLine(void);
   };
}

