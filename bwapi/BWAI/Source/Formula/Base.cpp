#include "Base.h"

#include <Util/Types.h>
#include <Util/Strings.h>

#include "Multiplication.h"
#include "Summation.h"
#include "Number.h"
#include "Function.h"

namespace Formula
{
  //------------------------------------------------ CONSTRUCTOR ---------------------------------------------
  Base::Base()
  {
  }
  //------------------------------------------------ DESTRUCTOR ----------------------------------------------
  Base::~Base()
  {
    for each (Base* i in nested)
      delete i;
  }
  //----------------------------------------------------------------------------------------------------------
  Base* Base::parseInternal(const std::string& input, size_t& position)
  {
    Summation* summation = new Summation();
    Multiplication* multiplication = new Multiplication();
    try
    {
      for ever
      {
        Util::Strings::skipSpace(input, position);
        switch (input[position])
        {
          case '(' : 
            position++;
            multiplication->nested.push_back(Base::parseInternal(input, ++position));
            if (input[position] != ')')
              throw ParseException("Not closed bracket in '" + input + "'");
            break;
          case 0 :
          case ')' :
            if (multiplication->nested.empty())
              delete multiplication;
            else if (multiplication->nested.size() == 1)
            {
              Base* item = multiplication->nested.front();
              multiplication->nested.clear();
              delete multiplication;
              multiplication = NULL;
              summation->nested.push_back(item);
            }
            else 
              summation->nested.push_back(multiplication);
            if (summation->nested.empty())
              throw ParseException("Empty expression in " + input);
            if (summation->nested.size() == 1)
            {
              Base* item = summation->nested.front();
              summation->nested.clear();
              delete summation;
              return item;
            }
            return summation;
          case '*' : break;
          case '+' :
            summation->nested.push_back(multiplication);
            multiplication = new Multiplication();
            break;
          case '-' :
            summation->nested.push_back(multiplication);
            multiplication = new Multiplication();          
            multiplication->nested.push_back(new Number(-1));            
            break;
          default : 
            if (isdigit(input[position]))
            {
              multiplication->nested.push_back(new Number(Util::Strings::readNumber(input, position)));
              position--;
            }
            else if (isalpha(input[position]))
              multiplication->nested.push_back(new Function(input, position));
            else
              throw ParseException((std::string)"Unknown character '" + input[position] + "'in formula '" + input + "' at position " + Util::Strings::intToString(position));
        } 
        position ++;
      }
    }
    catch (ParseException& e)
    {
      delete multiplication;
      delete summation;
      throw e;
    }
  }
  //----------------------------------------------------------------------------------------------------------
  Base* Base::parse(const std::string& input)
  {
    size_t position = 0;
    return parseInternal(input, position);
  }
  //----------------------------------------------------------------------------------------------------------
}