#include "Sentence.h"

#include "Strings.h"
#include "Dictionary.h"

namespace Util
{
  //-------------------------------- CONSTRUCTOR ---------------------------------
  Sentence::Sentence(const std::string& key, std::string sentence, Dictionary* dictionary, DictionaryFile *parentFile)
  :parentFile(parentFile)
  ,key(key)
  {
    this->inputValue = sentence;
    size_t leftBracketPosition, rightBracketPosition;
    std::string newNumberString;
    std::string value;
    size_t position;
    std::string NumberString;
    position = 0;
    begin1:
    leftBracketPosition = sentence.find_first_of("{\\",position);
    if (leftBracketPosition == -1)
      this->sentence = sentence;
    else
    {
      if (sentence[leftBracketPosition] == '\\')
      {
        switch (sentence[leftBracketPosition + 1])
        {
          case  '{' :
          {
            sentence = sentence.substr(0, leftBracketPosition - 1) + sentence.substr(leftBracketPosition, sentence.length() - leftBracketPosition);
            position = leftBracketPosition + 2;
            goto begin1;
          }
          case 'n' :
          {
            sentence = sentence.substr(0,leftBracketPosition-1) + "\n" +sentence.substr(leftBracketPosition+1,sentence.length() - leftBracketPosition - 1);
            position = leftBracketPosition + 2;
            goto begin1;
          }
          default :
          {
            sentence = sentence.substr(0,leftBracketPosition) + sentence.substr(leftBracketPosition + 1,sentence.length() - leftBracketPosition - 1);
            position = leftBracketPosition + 1;
            goto begin1;
          }
        }
      }
      this->sentence = sentence.substr(0,leftBracketPosition);
      rightBracketPosition = sentence.find("}",leftBracketPosition);
      size_t newrightBracketPosition;
      NumberString = sentence.substr(leftBracketPosition + 1,rightBracketPosition - leftBracketPosition - 1);
      
      if (NumberString.find_first_not_of("0123456789",0) == -1)
        while (true)
         {
          position = rightBracketPosition;
          begin2:
          leftBracketPosition = sentence.find_first_of("{\\",position);
          if (leftBracketPosition != -1)
          {
            if (sentence[leftBracketPosition] == '\\')
            {
              switch (sentence[leftBracketPosition + 1])
              {
                case  '{' :
                {
                  sentence = sentence.substr(0,leftBracketPosition) + sentence.substr(leftBracketPosition + 1,sentence.length() - leftBracketPosition - 1);
                  position = leftBracketPosition + 2;
                  goto begin2;
                }
                case 'n' :
                {
                  sentence = sentence.substr(0,rightBracketPosition) + sentence.substr(leftBracketPosition + 1,sentence.length() - leftBracketPosition - 1);
                  position = leftBracketPosition + 2;
                  goto begin2;
                }
                default :
                {
                  sentence = sentence.substr(0,rightBracketPosition) + sentence.substr(leftBracketPosition + 1,sentence.length() - leftBracketPosition - 1);
                  position = leftBracketPosition + 1;
                  goto begin2;
                }
              }
            }
            newrightBracketPosition = sentence.find("}",leftBracketPosition);
            newNumberString = sentence.substr(leftBracketPosition + 1,newrightBracketPosition - leftBracketPosition - 1);
            if (newNumberString.find_first_not_of("0123456789",0) == -1)
            {
              this->sentenceParts.push_back(std::pair<std::string, unsigned int>(std::string(sentence.substr(rightBracketPosition + 1,leftBracketPosition - rightBracketPosition - 1)),unsigned int(atoi(NumberString.c_str()) - 1)));
              rightBracketPosition = newrightBracketPosition;
              NumberString = newNumberString;
            }
            else
              if (dictionary == NULL || dictionary->values[NumberString] == NULL)
              {
                this->sentenceParts.push_back(std::pair<std::string,unsigned int>(std::string("Parametr must be NUMBER (error in Dictionary file)"), unsigned int(0)));
                return;
              }
              else
              {
                value = dictionary->get(NumberString);
                position = leftBracketPosition + value.length();
                sentence = sentence.substr(0,leftBracketPosition) + value + sentence.substr(position,sentence.length() - position);
                goto begin2;
              }
           }
           else
           {
            this->sentenceParts.push_back(std::pair<std::string,unsigned int>(std::string(sentence.substr(rightBracketPosition + 1,sentence.length() - rightBracketPosition - 1)),unsigned int(atoi(NumberString.c_str()) - 1)));
            return;
           }
         }
       else
         {
          if (dictionary == NULL || dictionary->values[NumberString] == NULL)
           {
            this->sentenceParts.push_back(std::pair<std::string,unsigned int>(std::string("Parametr must be NUMBER or DICTIONARY KEY (error in Dictionary file)"), unsigned int(0)));
            return;
           }
          else
           {
            value = dictionary->get(NumberString);
            position = leftBracketPosition + value.length();
            sentence = sentence.substr(0,leftBracketPosition) + value + sentence.substr(rightBracketPosition + 1 ,sentence.length() - rightBracketPosition - 1);
            goto begin1;
           }
         }
     }
   }
  //------------------------------------ SET VALUE -------------------------------
  void Sentence::setValue(const std::string& value)
   {
    /* If the basic value is same as the new one, and the macro doesn't have any
       parameters, the value doesn't need to updated */
    if (this->sentence == value && this->sentenceParts.size() != 0)
      return;

    this->sentence = value;
    this->sentenceParts.clear();
    this->inputValue = value;
    this->parentFile->setChanged(true);
   }
  //-------------------------------- GET LINE TEXT -------------------------------
  std::string Sentence::getLine(void)
   {
    return this->getKey() + " = " + this->inputValue;
   }
  //--------------------------------- DESTRUCTOR ---------------------------------
  Sentence::~Sentence(void)
   {
   }
  //---------------------------------- GET KEY -----------------------------------
  const std::string& Sentence::getKey(void) const
   {
    return this->key;
   }
  //-------------------------------- GET SENTENCE --------------------------------

  std::string Sentence::getSentenceMap()
   {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      returnValue += "{" + Strings::intToString(i.second) + "}";
      returnValue += i.first;
     }
    return returnValue;
   }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence()
  {
    std::string returnValue = this->sentence;
    
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      returnValue += " {Missing Argument} ";
      returnValue += i.first;
     }
    return returnValue;
  }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence(std::vector<std::string>* argumentList)
  {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      if (i.second <= argumentList->size())
        returnValue += (*argumentList)[(i.second)];
      else
        returnValue += "{MissingArgument}";
      returnValue +=  i.first;
     }
    return returnValue;
   }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence(const std::string& parameter1)
   {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      switch (i.second)
       {
        case 0  : returnValue += parameter1; break;
        default : returnValue += "{MissingArgument}";
       }
      returnValue += i.first;
     }
    return returnValue;
  }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence(const std::string& parameter1, const std::string& parameter2)
   {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      switch (i.second)
       {
        case 0  : returnValue += parameter1; break;
        case 1  : returnValue += parameter2; break;
        default : returnValue += "{MissingArgument}";
       }
      returnValue += i.first;
     }
    return returnValue;
  }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3)
   {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      switch (i.second)
       {
        case 0  : returnValue += parameter1; break;
        case 1  : returnValue += parameter2; break;
        case 2  : returnValue += parameter3; break;
        default : returnValue += "{MissingArgument}";
       }
      returnValue += i.first;
     }
    return returnValue;
  }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3, const std::string& parameter4)
   {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      switch (i.second)
       {
        case 0  : returnValue += parameter1; break;
        case 1  : returnValue += parameter2; break;
        case 2  : returnValue += parameter3; break;
        case 3  : returnValue += parameter3; break;
        default : returnValue += "{MissingArgument}";
       }
      returnValue += i.first;
     }
    return returnValue;
  }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3, const std::string& parameter4, const std::string& parameter5)
   {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      switch (i.second)
       {
        case 0  : returnValue += parameter1; break;
        case 1  : returnValue += parameter2; break;
        case 2  : returnValue += parameter3; break;
        case 3  : returnValue += parameter4; break;
        case 4  : returnValue += parameter5; break;
        default : returnValue += "{MissingArgument}";
       }
      returnValue += i.first;
     }
    return returnValue;
  }
  //-------------------------------- GET SENTENCE --------------------------------
  std::string Sentence::getSentence(const std::string& parameter1, const std::string& parameter2, const std::string& parameter3, const std::string& parameter4, const std::string& parameter5, const std::string& parameter6)
   {
    std::string returnValue = this->sentence;
    for each (std::pair<std::string,unsigned int> i in this->sentenceParts)
     {
      switch (i.second)
       {
        case 0  : returnValue += parameter1; break;
        case 1  : returnValue += parameter2; break;
        case 2  : returnValue += parameter3; break;
        case 3  : returnValue += parameter4; break;
        case 4  : returnValue += parameter5; break;
        case 5  : returnValue += parameter6; break;
        default : returnValue += "{MissingArgument}";
       }
      returnValue += i.first;
     }
    return returnValue;
  }
  //------------------------------------------------------------------------------
  const std::string& Sentence::getSentenceKey(const Sentence* const _this)
   {
    return _this->getKey();
   }
  //------------------------------------------------------------------------------
}

