#pragma once
extern int assert_success_count;
extern int assert_fail_count;
#define BWAssert(C)\
{\
  if (!(C))\
  {\
    Broodwar->printf("Assertion failed in file %s on line %d",__FILE__,__LINE__);\
    assert_fail_count++;\
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
    Broodwar->printf("Assertion failed in file %s on line %d: %s",__FILE__,__LINE__,E);\
    assert_fail_count++;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}