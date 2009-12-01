#include "FileLineWriter.h"

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
    int failure = fopen_s(&this->handle, filePath.c_str(), "w");
    if(failure)
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
