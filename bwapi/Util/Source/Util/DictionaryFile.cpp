#include "DictionaryFile.h"
#include "Exceptions.h"
#include "DictionaryLine.h"
#include "Sentence.h"
#include "Strings.h"
#include "Dictionary.h"
#include "UnusedLine.h"
#include "Foreach.h"

namespace Util
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  DictionaryFile::DictionaryFile(const std::string &fileName, Dictionary *dictionary)
   {
     this->fileName = fileName;
     FILE* f = fopen(fileName.c_str(),"rt");
     if (f)
     {
      std::string::size_type endOfKey, endOfTrimmedKey;
      std::string line;
      Sentence* sentence;
      while (!feof(f))
       {
        std::string line = Strings::readLine(f);
        endOfKey = line.find('=');
        if (endOfKey == 0)
          throw ParseException("Wrong config line '" + line + "'");
        if (endOfKey != std::string::npos)
         {
          endOfTrimmedKey = endOfKey - 1;
          while (endOfTrimmedKey != 0 && isspace(line[endOfTrimmedKey]))
            endOfTrimmedKey--;
          endOfKey++;
          while (isspace(line[endOfKey]))
            endOfKey++;
          sentence = new Sentence(line.substr(0, endOfTrimmedKey + 1), 
                                  line.substr(endOfKey, line.length() - endOfKey), 
                                  dictionary, 
                                  this);
          if (dictionary != NULL)
            dictionary->values.insert(std::pair<std::string, Sentence*>(sentence->getKey(), sentence));
          this->lines.push_back(sentence);
          this->usedLines.push_back(sentence);
         }
        else
          this->lines.push_back(new UnusedLine(line));
       }
      fclose(f);
      this->changed = false; 
     }
    else
      throw new FileException("Unable to open dicionary file " + fileName);
   }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  DictionaryFile::~DictionaryFile(void)
  {
    foreach (DictionaryLine* i, this->lines)
      delete i;
  }
  //---------------------------------------------- SAVE TO FILE ----------------------------------------------
  void DictionaryFile::saveToFile(void)
  {
    if (!this->changed)
      return;
    FILE *f = fopen(fileName.c_str(),"wt");
    if (!f)
      throw new FileException("Couldn't open file for reading " + fileName);

    std::string line;
    foreach (DictionaryLine* i, this->lines)
    {
      if (i != this->lines.front())
        fwrite("\n",sizeof(char),1,f);
      line = i->getLine();
      fwrite(line.c_str(),sizeof(char)*line.length(),1,f);
    }
    fclose(f);
  }
  //----------------------------------------------------------------------------------------------------------
  void DictionaryFile::setChanged(const bool changed)
  {
    this->changed = changed;
  }
  //----------------------------------------------------------------------------------------------------------
}
