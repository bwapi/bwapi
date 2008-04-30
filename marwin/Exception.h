#ifndef EXCEPTIONH
#define EXCEPTIONH
#include <string>
class Exception
 {
  public :
    Exception(const std::string & const message);
    ~Exception();
   std::string getMessage(void);
  private :
    std::string message;
 };

#endif