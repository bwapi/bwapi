/*****************************************************************************
 *  The Interface Packer
 *  - reads ini file
 *  - copies all target files to target folder
 *  - changes the #include statements in all those files
 *  - hides any members or globals not marked as BWAPI_INTERFACE
 *  - creates an all.h that includes all files.
 *
 ******/

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <Util\Types.h>
#include <Util\Buffer.h>
#include <Util\MemoryFrame.h>
#include <Util\FileLineReader.h>
#include <Util\FileLineWriter.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <set>

#define EXIT(...)       \
{                       \
  printf(__VA_ARGS__);  \
  printf("\n");         \
  system("pause");      \
  exit(1);              \
}

std::string outputDirectory;
std::string collectorName;
std::set<std::string> targetFiles;   // files to transit
std::set<std::string> includePaths;
std::set<std::string> processedFiles;

std::set<std::string> missingFiles;
//----------------------------- READ INI FILE ------------------------------------------------------------
void readIniFile(std::string iniFileName)
{
  Util::FileLineReader reader;
  if(!reader.openFile(iniFileName))
    EXIT("could not load file");

  std::string line = reader.readLine();
  if(line != ":output directory")
    EXIT("':output directory' expected");

  outputDirectory = reader.readLine();

  line = reader.readLine();
  if(line != ":collector name")
    EXIT("':collector name' expected");

  collectorName = reader.readLine();

  line = reader.readLine();
  if(line != ":include directories")
    EXIT("':include directories' expected");

  while(!reader.isEof())
  {
    line = reader.readLine();
    if(line[0] == ':')
    {
      if(line != ":target files")
        EXIT("':target files' expected at line %d", reader.getLineNumber());
      break;
    }
    if(line.size() == 0)
      continue;
    includePaths.insert(line);
  }
  while(!reader.isEof())
  {
    line = reader.readLine();
    if(!line.size())
      continue;
    if(line[0] == ':')
    {
      EXIT("':' in '%s' no expected on line %d", line.c_str(), reader.getLineNumber());
      break;
    }
    if(line.size() == 0)
      continue;
    targetFiles.insert(line);
  }
}
//----------------------------- FILE EXISTS --------------------------------------------------------------
bool fileExists(std::string filename)
{
  return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}
//----------------------------- GET FILE NAME FROM PATH --------------------------------------------------
std::string fileNameFromPath(std::string filePath)
{
  for(int head = filePath.size()-1; head >= 0; head--)
  {
    if(filePath[head] == '\\' || filePath[head] == '/')
    {
      head++;
      return filePath.substr(head, filePath.size() - head);
    }
  }
  return filePath;  // no slashes found => all is file name
}
//----------------------------- GET FILE DIRECTORY FROM PATH ---------------------------------------------
std::string fileDirectoryFromPath(std::string filePath)
{
  for(int head = filePath.size()-1; head >= 0; head--)
  {
    if(filePath[head] == '\\' || filePath[head] == '/')
    {
      return filePath.substr(0, head);
    }
  }
  return "";  // no slashes found => no directory
}
//----------------------------- GET FILE PREFIX FROM PATH ------------------------------------------------
std::string filePrefixFromPath(std::string filePath)
{
  std::string fileDirectory = fileDirectoryFromPath(filePath);
  std::string prefix = "a";
  int id = 0;
  for(unsigned int i = 0; i < fileDirectory.size(); i++)
  {
    id += fileDirectory[i];
  }
  char buffer[100];
  _itoa_s(id, buffer, 100, 30);
  prefix += std::string(buffer);
  return prefix;
}
//----------------------------- FIND FILE PATH -----------------------------------------------------------
std::string findFilePath(std::string fileName, std::string additionalIncludePath = "")
{
  if(additionalIncludePath.size())
  {
    std::string fullPath = additionalIncludePath + "\\" + fileName;
    if(fileExists(fullPath))
    {
      return fullPath;
    }
  }
  for each(std::string includePath in includePaths)
  {
    std::string fullPath = includePath + "\\" + fileName;
    if(fileExists(fullPath))
    {
      return fullPath;
    }
  }
  return "";
}
//----------------------------- PROCESS FILE -------------------------------------------------------------
void processFile(std::string sourceFilePath, std::string destFilePath)
{
  if(fileExists(destFilePath))
  {
    EXIT("file already exists: %s\n", destFilePath.c_str());
  }

  // file IO means
  Util::FileLineReader reader;
  Util::FileLineWriter writer;
  if(!reader.openFile(sourceFilePath))
  {
    EXIT("error opening source '%s'", sourceFilePath.c_str());
  }
  if(!writer.openFile(destFilePath))
  {
    EXIT("error opening dest '%s'", destFilePath.c_str());
  }

  // parameter deductions
  std::string fileDirectory = fileDirectoryFromPath(sourceFilePath);
  std::string filePrefix = filePrefixFromPath(sourceFilePath);

  // copy line per line
  std::set<std::string> filter;
  bool strip = false;
  bool ignore = false;
  filter.insert("#"); // line beginnings that withstand stripping
  filter.insert("{");
  filter.insert("}");
  filter.insert("//");
  filter.insert("/*");
  filter.insert("*/");
  filter.insert("class");
  filter.insert("public");
  filter.insert("namespace");
  filter.insert("BWAPI2_FUNCTION");
  filter.insert("BWAPI2_METHOD");
  while(true)
  {
    std::string line = reader.readLine();
    unsigned int firstNonSpace = line.find_first_not_of(' ');

    // modus changer commands
    if(firstNonSpace != std::string::npos)
    {
      if(line.substr(firstNonSpace, 8) == "IP_STRIP")
      {
        strip = true;
        ignore = false;
        continue;
      }
      if(line.substr(firstNonSpace, 9) == "IP_IGNORE")
      {
        strip = false;
        ignore = true;
        continue;
      }
      if(line.substr(firstNonSpace, 10) == "IP_TRANSIT")
      {
        strip = false;
        ignore = false;
        continue;
      }
    }
    if(ignore)
      continue;

    // fix include statements to local paths
    if(line.substr(0, 8) == "#include")
    {
      int from = line.find_first_of('"');
      int to   = line.find_last_of('"');
      int from2= line.find_first_of('<');
      int to2  = line.find_last_of('>');
      bool relative = false;
      if(from2 != to2)
      {
        from = from2;
        to = to2;
        relative = true;
      }
      if(to != from)
      {
        // extract relative file name
        std::string includeeRelativeFileName = line.substr(from+1, to-(from+1));

        // find the file
        std::string includeeFilePath = findFilePath(includeeRelativeFileName, fileDirectory);

        // extract the file name
        std::string includeeFileName = fileNameFromPath(includeeFilePath);

        std::string includeePrefix;
        if(includeeFilePath == "") // if not found, leave it as is
        {
          if(missingFiles.find(includeeRelativeFileName) == missingFiles.end())
          {
            printf("missing include: '%s' in '%s'\n", includeeRelativeFileName.c_str(), sourceFilePath.c_str());
            missingFiles.insert(includeeRelativeFileName);
          }
          includeePrefix = "";
          includeeFileName = includeeRelativeFileName;
        }
        else
        {
          // generate included file prefix
          includeePrefix = filePrefixFromPath(includeeFilePath);
        }

        // splice folders out of the path
        line = line.substr(0, from) + "\"" + includeePrefix + includeeFileName + "\"" + line.substr(to+1, line.size()-(to+1));
      }
    }

    // filter strip
    if(strip)
    {
      if(firstNonSpace != -1)
      {
        // find match with any filter string
        bool match = false;
        std::string matchString;
        for each(std::string filterString in filter)
        {
          if(line.substr(firstNonSpace, filterString.size()) == filterString)
          {
            match = true;
            matchString = filterString;
            break;
          }
        }
        if(!match)
          continue; // ignore the line
        if(matchString == "BWAPI2_METHOD" || matchString == "virtual")
        {
          // make virtual functions pure
          std::string pureEnd = ") = 0;";
          std::string pureEndConst = ") const = 0;";
          if(line.find('~') != std::string::npos)
          {
            // this is a destructor. those may not be pure (or const)
            pureEnd = "){};";
          }

          // characters to replace
          // note there is often a comment at the end
          unsigned int endPos = line.find(");");
          unsigned int endPosConst = line.find(") const;");
          if(endPos       != std::string::npos)
          {
            line = line.substr(0, endPos)       + pureEnd       + line.substr(endPos+2);
          }
          if(endPosConst  != std::string::npos)
          {
            line = line.substr(0, endPosConst)  + pureEndConst  + line.substr(endPosConst+8);
          }
        }
      }
    }

    writer.writeLine(line);
    // eof
    if(reader.isEof())
      break;
  }
//  printf("%s: %s => %s\n", strip ? "strip" : "transit", sourceFilePath.c_str(), destFilePath.c_str());
}
//----------------------------- COPY FILES ---------------------------------------------------------------
void copyFiles()
{
  for each(std::string relativeFileName in targetFiles)
  {
    std::string filePath = findFilePath(relativeFileName);
    if(filePath == "")
    {
      printf("could not find target file '%s'\n", relativeFileName.c_str());
      continue;
    }
    std::string fileName = fileNameFromPath(filePath);
    std::string filePrefix = filePrefixFromPath(filePath);
    processFile(filePath, outputDirectory + "\\" + filePrefix + fileName);
    processedFiles.insert(filePrefix + fileName);
  }
}
//----------------------------- GENERATE ALL H -----------------------------------------------------------
void generateAllX(std::string extension)
{
  Util::FileLineWriter writer;
  writer.openFile(outputDirectory + "\\" + collectorName + "." + extension);
  if(extension == "h")
    writer.writeLine("#pragma once");
  for each(std::string fileName in processedFiles)
  {
    if(fileName.substr(fileName.size()-extension.size(), extension.size()) != extension)
      continue;
    writer.writeLine("#include \"" + fileName + "\"");
  }
}
//----------------------------- MAIN ---------------------------------------------------------------------
int _tmain(int argc, char* argv[])
{
  char* initFileName = argv[1];
  if(!initFileName || initFileName[0] == 0)
    return 1;
  if(!fileExists(initFileName))
  {
    printf("could not find ini file %s", initFileName);
    system("pause");
    return 1;
  }
  readIniFile(initFileName);
  copyFiles();
  generateAllX("h");
  generateAllX("cpp");
  system("pause");
  return 0;
}
//----------------------------- --------------------------------------------------------------------------

