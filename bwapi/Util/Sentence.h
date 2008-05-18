#pragma once

#include "DictionaryLine.h"
#include <list>
#include <vector>
class Dictionary;
class DictionaryFile;

/** 
 * Represents one property value.
 * the representation of the property value can be for example: <br>
 * cant_open_file_message=File {1} couldn't be opened, error message is '{2}' <br>
 * This property has two parameters, so it will be represnted by this class with the following data.
 * sentence="File "
 * sentenceParts={"couldn't be opened,error message is '"1},{"'",2}
 */
class Sentence : public DictionaryLine
 {
  private :
   std::string key, sentence;
   std::string inputValue;
   std::list<std::pair<std::string,unsigned int>> sentenceParts;
   DictionaryFile *parentFile;
  public :
   Sentence(const std::string& key, std::string sentence, Dictionary* dictionary, DictionaryFile *parentFile);
   virtual ~Sentence(void);
   void setValue(const std::string& value);
   std::string getLine(void);
   const std::string& getKey() const;
   std::string getSentence();
   std::string getSentenceMap();
   std::string getSentence(std::vector<std::string>* argumentList);
   std::string getSentence(const std::string& parameter1);
   std::string getSentence(const std::string& parameter1, const std::string& parameter2);
   std::string getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3);
   std::string getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3, const std::string& parameter4);
   std::string getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3, const std::string& parameter4, const std::string& parameter5);
   std::string getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3, const std::string& parameter4, const std::string& parameter5, const std::string& parameter6);
   static const std::string &getSentenceKey(const Sentence* const _this);
};
