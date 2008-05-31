#include "DictionaryFile.h"
#include "Exceptions.h"
#include "DictionaryLine.h"
#include "Sentence.h"
#include "Strings.h"
#include "Dictionary.h"
#include "UnusedLine.h"

namespace Util
{
  //---------------------------------- CONSTRUCTOR -------------------------------
  DictionaryFile::DictionaryFile(const std::string &fileName, Dictionary *dictionary)
   {
     this->fileName = fileName;

     FILE* f = fopen(fileName.c_str(),"rt");
     if (f)
     {
      size_t endOfKey;
      std::string line, key, value;
      Sentence* sentence;
      while (!feof(f))
       {
        std::string line = Strings::readLine(f);
        endOfKey = line.find("=");
        if (endOfKey != -1)
         {
          key = line.substr(0, endOfKey);
          while (isspace(key[key.length() -1]))
            key = key.substr(0, key.length() - 1);
          value = line.substr(endOfKey + 1,line.length() - endOfKey - 1);
	      	  while (isspace(value[0]))
            value = value.substr(1,value.length() - 1);
          sentence = new Sentence(key, value, dictionary, this);
          dictionary->values.insert(std::pair<std::string, Sentence*>(key, sentence));
          this->lines.push_back(sentence);
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
  //----------------------------------- DESTRUCTOR -------------------------------
  DictionaryFile::~DictionaryFile(void)
  {
    for (std::list<DictionaryLine*>::iterator i = this->lines.begin(); i != this->lines.end(); ++i)
      delete *i;
  }
  //---------------------------------- SAVE TO FILE ------------------------------
  void DictionaryFile::saveToFile(void)
  {
    if (!this->changed)
      return;
    FILE *f = fopen(fileName.c_str(),"wt");
    if (!f)
      throw new FileException("Couldn't open file for reading " + fileName);

    std::string line;
    for (std::list<DictionaryLine*>::iterator i = this->lines.begin(); i != this->lines.end(); ++i)
    {
      if (i != lines.begin())
        fwrite("\n",sizeof(char),1,f);
      line = (*i)->getLine();
      fwrite(line.c_str(),sizeof(char)*line.length(),1,f);
    }
    fclose(f);
  }
  //------------------------------------------------------------------------------
  void DictionaryFile::setChanged(const bool changed)
  {
    this->changed = changed;
  }
  //------------------------------------------------------------------------------
}