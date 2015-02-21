#ifndef STARCRAFT_VER
#define STARCRAFT_VER "1.16.1"

#define SC_VER_1 1
#define SC_VER_2 16
#define SC_VER_3 1
#define SC_VER_4 1

#ifdef _DEBUG
  #define BUILD_STR "DEBUG"
  #define BUILD_DEBUG 1
#else
  #define BUILD_STR "RELEASE"
  #define BUILD_DEBUG 0
#endif

#define BWAPI_HOME_URL "http://bwapi.github.io/"
#endif
