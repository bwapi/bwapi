#pragma once
////////////////////////////////////////////////////////////////////////
//                                                                    //
//                                                                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { class FileLineWriter; }

#include <string>
#include <stdlib.h>

namespace Util
{
  class FileLineWriter
  {
  public:
    FileLineWriter();
    ~FileLineWriter();

    bool openFile(std::string filePath);
    void writeLine(std::string);
    int getLineNumber() const;
    void close();

  private:
    FILE *handle;
    int lineNumber;
  };
}
