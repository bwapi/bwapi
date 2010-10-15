#pragma once
extern int assert_success_count;
extern int assert_fail_count;
void log(const char* text, ...);
#define BWAssert(C)\
{\
  if (!(C))\
  {\
    log("Assertion failed in file %s on line %d",__FILE__,__LINE__);\
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
    log("Assertion failed in file %s on line %d",__FILE__,__LINE__);\
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
    log("Assertion failed in file %s on line %d: %s",__FILE__,__LINE__,E);\
    assert_fail_count++;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}