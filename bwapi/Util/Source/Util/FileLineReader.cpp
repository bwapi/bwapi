#include "FileLineReader.h"
#include <stdio.h>
#include <string.h>

namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  FileLineReader::FileLineReader()
    : lineNumber(0)
  {
  }

  FileLineReader::~FileLineReader()
  {
  }
  //----------------------- OPEN FILE ----------------------------------
  bool FileLineReader::openFile(std::string filePath)
  {
    this->handle = fopen(filePath.c_str(), "r");
    if(!this->handle)
      return false;
    lineNumber = 0;
    return true;
  }
  //----------------------- READ LINE ----------------------------------
  std::string FileLineReader::readLine()
  {
    char buff[512];
    if(!fgets(buff, 200, this->handle))
      buff[0] = 0;
    this->lineNumber++;
    int length = strlen(buff);
    if(buff[length-1] == '\n')
      buff[length-1] = 0;
    return std::string(buff);
  }
  //----------------------- IS LAST ------------------------------------
  bool FileLineReader::isEof() const
  {
    return !!feof(this->handle);
  }
  //----------------------- GET LINE NUMBER ----------------------------
  int FileLineReader::getLineNumber() const
  {
    return this->lineNumber;
  }
  //----------------------- CLOSE --------------------------------------
  void FileLineReader::close()
  {
    fclose(this->handle);
  }
  //----------------------- --------------------------------------------
}
