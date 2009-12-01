#pragma once
////////////////////////////////////////////////////////////////////////
//                                                                    //
//                                                                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { class FileLineReader; }

#include <string>
#include <stdlib.h>

namespace Util
{
  class FileLineReader
  {
  public:
    FileLineReader();
    ~FileLineReader();

    bool openFile(std::string filePath);
    std::string readLine();
    int getLineNumber() const;
    bool isEof() const;
    void close();

  private:
    FILE *handle;
    int lineNumber;
  };
}
