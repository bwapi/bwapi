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

#define EXIT(...)      \
{                       \
  printf(__VA_ARGS__);  \
  printf("\n");         \
  system("pause");      \
  exit(1);              \
}

std::string outputDirectory;
std::string collectorName;
std::set<std::string> stripFiles;   // strip theese of all unnecessarity
std::set<std::string> transitFiles; // these are user's side classes, don't strip these, just correct the path
std::set<std::string> includePaths;
std::set<std::string> processedFiles;
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
      if(line != ":strip files")
        EXIT("':strip files' expected at line %d", reader.getLineNumber());
      break;
    }
    if(line.size() == 0)
      continue;
    includePaths.insert(line);
  }
  while(!reader.isEof())
  {
    line = reader.readLine();
    if(line[0] == ':')
    {
      if(line != ":transit files")
        EXIT("':transit files' expected at line %d", reader.getLineNumber());
      break;
    }
    if(line.size() == 0)
      continue;
    stripFiles.insert(line);
  }
  while(!reader.isEof())
  {
    line = reader.readLine();
    if(line[0] == ':')
    {
      EXIT("':' in '%s' no expected on line %d", line.c_str(), reader.getLineNumber());
      break;
    }
    if(line.size() == 0)
      continue;
    transitFiles.insert(line);
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
  return filePath;
}
//----------------------------- FIND FILE PATH -----------------------------------------------------------
std::string findFilePath(std::string fileName)
{
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
void processFile(std::string sourceFilePath, std::string destFilePath, bool strip)
{
  if(fileExists(destFilePath))
  {
    EXIT("file already exists: %s\n", destFilePath.c_str());
  }
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
  printf("%s: %s => %s\n", strip ? "strip" : "transit", sourceFilePath.c_str(), destFilePath.c_str());

  // copy line per line
  std::set<std::string> filter;
  filter.insert("#");
  filter.insert("{");
  filter.insert("}");
  filter.insert("};");
  filter.insert("//");
  filter.insert("/*");
  filter.insert("*/");
  filter.insert("class");
  filter.insert("public");
  filter.insert("namespace");
  filter.insert("AGENT_API");
  filter.insert("AGENT_INTERFACE");
  filter.insert("virtual");   // just temporarily
  while(true)
  {
    std::string line = reader.readLine();
    int firstNonSpace = line.find_first_not_of(' ');

    // fix include statements to local paths
    if(line.substr(0, 8) == "#include")
    {
      int from = line.find_first_of('"');
      int to   = line.find_last_of('"');
      int from2= line.find_first_of('<');
      int to2  = line.find_last_of('>');
      if(from2 != to2)
      {
        from = from2;
        to = to2;
      }
      if(to != from)
      {
        // splice folders out of the path
        std::string includee = line.substr(from+1, to-(from+1));
        line = line.substr(0, from) + "\"" + fileNameFromPath(includee) + "\"" + line.substr(to+1, line.size()-(to+1));
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
        if(matchString == "AGENT_INTERFACE" || matchString == "virtual")
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
          int endPos = line.find(");");
          int endPosConst = line.find(") const;");
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
}
//----------------------------- COPY FILES ---------------------------------------------------------------
void copyFiles()
{
  for each(std::string relativeFileName in stripFiles)
  {
    std::string filePath = findFilePath(relativeFileName);
    if(filePath == "")
    {
      printf("could not find strip file '%s'\n", relativeFileName);
      continue;
    }
    std::string fileName = fileNameFromPath(filePath);
    processFile(filePath, outputDirectory + "\\" + fileName, true);
    processedFiles.insert(fileName);
  }
  for each(std::string relativeFileName in transitFiles)
  {
    std::string filePath = findFilePath(relativeFileName);
    if(filePath == "")
    {
      printf("could not find transit file '%s'\n", relativeFileName);
      continue;
    }
    std::string fileName = fileNameFromPath(filePath);
    processFile(filePath, outputDirectory + "\\" + fileName, false);
    processedFiles.insert(fileName);
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
int _tmain(int argc, _TCHAR* argv[])
{
  readIniFile("details.ini");
  copyFiles();
  generateAllX("h");
  generateAllX("cpp");
  system("pause");
  return 0;
}
//----------------------------- --------------------------------------------------------------------------

