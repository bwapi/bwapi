#include "Dictionary.h"
#include "Sentence.h"
#include "UnusedLine.h"
#include "DictionaryFile.h"
#include "StringUtil.h"
#include <algorithm>
//-------------------------------- CONSTRUCTOR ---------------------------------
Dictionary::Dictionary(const std::string &fileName)
{
  this->load(fileName);
}
//---------------------------------- LOAD ------------------------------------
void Dictionary::load(const std::string &fileName)
{
  this->files.push_back(new DictionaryFile(fileName, this));
}
//---------------------------------- SAVE TO FILE ------------------------------
void Dictionary::saveToFile(void)
{
  for (std::list<DictionaryFile*>::iterator i = this->files.begin(); i!= this->files.end(); ++i)
  (*i)->saveToFile();
}
//--------------------------------- DESTRUCTOR ---------------------------------
Dictionary::~Dictionary()
 {
  for (std::list<DictionaryFile*>::iterator i = this->files.begin(); i!= this->files.end(); ++i)
   delete *i;
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence();
  else
    return "{Key '"+ key + "'not found}";
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key, std::vector<std::string>* argumentList)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence(argumentList);
  else
    return "{Key '"+ key + "'not found}";
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key, const std::string &parameter1)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence(parameter1);
  else
    return "{Key '"+ key + "'not found}";
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key,const std::string &parameter1,const std::string &parameter2)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence(parameter1,parameter2);
  else
    return "{Key '"+ key + "'not found}";
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key,const std::string &parameter1,const std::string &parameter2,const std::string &parameter3)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence(parameter1,parameter2,parameter3);
  else
    return "{Key '"+ key + "'not found}";
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key,const std::string &parameter1,const std::string &parameter2,const std::string &parameter3,const std::string &parameter4)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence(parameter1,parameter2,parameter3,parameter4);
  else
    return "{Key '"+ key + "'not found}";
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key,const std::string &parameter1,const std::string &parameter2,const std::string &parameter3,const std::string &parameter4,const std::string &parameter5)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence(parameter1,parameter2,parameter3,parameter4,parameter5);
  else
    return "{Key '"+ key + "'not found}";
 }
//--------------------------------- GET VALUE ----------------------------------
std::string Dictionary::getValue(const std::string &key,const std::string &parameter1,const std::string &parameter2,const std::string &parameter3,const std::string &parameter4,const std::string &parameter5,const std::string &parameter6)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence(parameter1,parameter2,parameter3,parameter4,parameter5,parameter6);
  else
    return "{Key '"+ key + "'not found}";
 }
//---------------------------------- GET NAME ----------------------------------
std::string Dictionary::getName(void)
 {
  return this->name;
 }
//---------------------------------- SET NAME ----------------------------------
void Dictionary::setName(const std::string &name)
 {
  this->name = name;
 }
//------------------------------- GET VALUE SAFE -------------------------------
std::string Dictionary::getValueSafe(const std::string &key)
 {
  Sentence* sentence = this->values[key];
  if (sentence != NULL)
    return sentence->getSentence();
   else
    return key;
 }
//--------------------------------- CLEAR ALL ----------------------------------
void Dictionary::clearAll(void)
{
  for (std::list<DictionaryFile*>::iterator i = this->files.begin(); i!= this->files.end(); ++i)
   delete *i;
  files.clear();
}
//------------------------------------------------------------------------------
