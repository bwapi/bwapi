#include "Exception.h"
//------------------------------- CONSTRUCTOR ----------------------------------
Exception::Exception(const std::string & const message)
:message(message)
 {
 }
//------------------------------- DESTRUCTOR -----------------------------------
Exception::~Exception(void)
 {
 }
//------------------------------- GET MESSAGE ----------------------------------
std::string Exception::getMessage(void)
 {
  return this->message;
 }
//------------------------------------------------------------------------------
