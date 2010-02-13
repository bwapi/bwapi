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
  printf("fatal: ");    \
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
std::set<std::string> stripLines;
std::set<std::string> unchangedIncludes;

std::set<std::string> missingFiles;
//----------------------------- READ INI FILE ------------------------------------------------------------
/**
 * parses the ini file and fills the containers with data
 */
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
  if(line != ":strip lines")
    EXIT("':strip lines' expected");

  while(!reader.isEof())
  {
    line = reader.readLine();
    if(line[0] == ':')
    {
      if(line != ":unchanged includes")
        EXIT("':unchanged includes' expected at line %d", reader.getLineNumber());
      break;
    }
    if(line.size() == 0)
      continue;
    stripLines.insert(line);
  }
  while(!reader.isEof())
  {
    line = reader.readLine();
    if(line[0] == ':')
    {
      if(line != ":include directories")
        EXIT("':include directories' expected at line %d", reader.getLineNumber());
      break;
    }
    if(line.size() == 0)
      continue;
    unchangedIncludes.insert(line);
  }
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
/**
 * returns true if the file exists
 */
bool fileExists(std::string filename)
{
  return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}
//----------------------------- STRIP FILE PATH ----------------------------------------------------------
/**
 * When concatinating multiple or complex file paths, the resulting path often has redundant folder
 * information. E.g.:
 * ..\Forlder1\..\Folder2\file.cpp
 * In this case Folder 1 is redundant because it is followed by the .. directive.
 * This functions removes all redundant path information.
 */
std::string stripFilePath(std::string filePath)
{
  // removes a folder if it is succeeded by a .. dive
  unsigned int diver = filePath.find("\\..\\");
  if(diver == std::string::npos)
    diver = filePath.find("\\../");
  if(diver == std::string::npos)
    diver = filePath.find("/..\\");
  if(diver == std::string::npos)
    diver = filePath.find("/../");

  // if there is a /../ string
  if(diver != std::string::npos)
  {
    // find the corresponding folder
    unsigned int boat = filePath.substr(0, diver).find_last_of("\\");

    if(boat == std::string::npos)
      boat = filePath.substr(0, diver).find_last_of("/");

    if(boat == std::string::npos)
      boat = 0; // if there is no slash, the folder is the first one in path
    else
      boat++; // point to first char in folder name

    // the folder may not be another dive, 2 dives do not cancel each other out
    if(filePath.substr(boat, diver-boat) == "..")
      return filePath;

    return stripFilePath(filePath.substr(0, boat) + filePath.substr(diver+4));
  }
  return filePath;
}
//----------------------------- GET FILE NAME FROM PATH --------------------------------------------------
/**
 * returns the filename substring of the path
 */
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
/**
 * returns the substring of everything up to the filename
 */
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
/**
 * Generates a string from a directory path. The string does not contain any special characters and
 * can be used in filenames
 */
std::string filePrefixFromPath(std::string filePath)
{
  if(false)
  {
    // verbose variant, but debuggable!
    std::string fileDirectory = fileDirectoryFromPath(filePath);
    std::string prefix = "";
    for(unsigned int i = 0; i < fileDirectory.size(); i++)
    {
      char c = fileDirectory[i];
      if(c == '\\' || c == '/')
        c = '_';
      if(c == '.')
        c = 'o';
      prefix += c;
    }
    return prefix + "_";
  }
  else
  {
    // compact variant
    std::string fileDirectory = fileDirectoryFromPath(filePath);
    std::string prefix = "a";
    int id = 0;
    for(unsigned int i = 0; i < fileDirectory.size(); i++)
    {
      char c = fileDirectory[i];
      if(c == '/' || c == '/')
        c = '\\';
      if(c >= 'A' && c <= 'Z')
        c += 'a' - 'A';
      id += fileDirectory[i];
    }
    char buffer[100];
    _itoa_s(id, buffer, 100, 30);
    prefix += std::string(buffer);
    return prefix;
  }
}
//----------------------------- FIND FILE PATH -----------------------------------------------------------
/**
 * searches for the file with fileName in all includePaths
 * returns the full path of the file if found
 * returns empty string if file not found
 */
std::string findFilePath(std::string fileName, std::string additionalIncludePath = "")
{
  if(additionalIncludePath.size())
  {
    std::string fullPath = stripFilePath(additionalIncludePath + "\\" + fileName);
    if(fileExists(fullPath))
    {
      return fullPath;
    }
  }
  for each(std::string includePath in includePaths)
  {
    std::string fullPath = stripFilePath(includePath + "\\" + fileName);
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
  bool inPrivate = false;
  filter.insert("#"); // line beginnings that withstand stripping
  filter.insert("{");
  filter.insert("}");
  filter.insert("//");
  filter.insert("/*");
  filter.insert("*/");
  filter.insert("class");
  filter.insert("public");
  filter.insert("namespace");
  filter.insert("BWSL_EXPORT");
  for(;;)
  {
    // end of file break condition
    if(reader.isEof())
    {
      if(inPrivate) // strip == true is implicit
      {
        printf("WARNING probably unwanted stripping behaviour:\n");
        printf("missing 'public:' after 'private:' in '%s'.\n", sourceFilePath.c_str());
      }
      break;
    }

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

    if(strip)
    {
      // when stripping, these selective lines of code will be ignored in all files
      if(stripLines.find(line) != stripLines.end())
        continue;
    }

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

        // check whether this is an include to leave unchanged
        if(unchangedIncludes.find(includeeRelativeFileName) == unchangedIncludes.end())
        {
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
    }

    // filter strip
    if(strip)
    {
      if(line.find("private:") != -1)
        inPrivate = true;
      if(line.find("public:") != -1)
        inPrivate = false;

      if(inPrivate)
        continue;

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
        if(matchString == "BWSL_METHOD" || matchString == "virtual")
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
  }
//  uncomment to see what files are being processed
//  printf("%s => %s\n", sourceFilePath.c_str(), destFilePath.c_str());
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
  printf("----------------------------------------------------------------------\n");
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
  printf("----------------------------------------------------------------------\n");
  printf("Interface Packer finished.\n");
  system("pause");
  return 0;
}
//----------------------------- --------------------------------------------------------------------------

