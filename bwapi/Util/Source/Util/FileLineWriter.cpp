#include "FileLineWriter.h"
#include <stdio.h>
namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  FileLineWriter::FileLineWriter()
    : lineNumber(0)
  {
  }

  FileLineWriter::~FileLineWriter()
  {
    fclose(this->handle);
  }
  //----------------------- OPEN FILE ----------------------------------
  bool FileLineWriter::openFile(std::string filePath)
  {
    this->handle = fopen(filePath.c_str(), "w");
    if(!this->handle)
      return false;
    lineNumber = 0;
    return true;
  }
  //----------------------- READ LINE ----------------------------------
  void FileLineWriter::writeLine(std::string line)
  {
    fputs((line + "\n").c_str(), this->handle);
    this->lineNumber++;
    return;
  }
  //----------------------- GET LINE NUMBER ----------------------------
  int FileLineWriter::getLineNumber() const
  {
    return this->lineNumber;
  }
  //----------------------- CLOSE --------------------------------------
  void FileLineWriter::close()
  {
    fclose(this->handle);
  }
  //----------------------- --------------------------------------------
}
