#pragma once
extern int assert_success_count;
extern int assert_fail_count;
void log(const char* format, ...);
#define BWAssert(C)\
{\
  if (!(C))\
  {\
    log("Assert failed @%s:%u (%s)",__FILE__,__LINE__,Broodwar->getLastError().toString().c_str());\
    assert_fail_count++;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}
#define BWAssertF(C,FailBlock)\
{\
  if (!(C))\
  {\
    log("Assert failed @%s:%u (%s)",__FILE__,__LINE__,Broodwar->getLastError().toString().c_str());\
    assert_fail_count++;\
    {FailBlock}\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}
#define BWAssertErr(C,E)\
{\
  if (!(C))\
  {\
    log("Assert failed @%s:%u (%s)",__FILE__,__LINE__,E);\
    assert_fail_count++;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}