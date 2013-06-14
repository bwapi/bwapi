@echo off
if not exist svnrev.h (
>> svnrev.h echo #define SVN_REV 1111
>> svnrev.h echo #define SVN_REV_STR "1111"
>> svnrev.h echo.
>> svnrev.h echo #ifdef _DEBUG
>> svnrev.h echo   #define BUILD_DEBUG 1
>> svnrev.h echo #else
>> svnrev.h echo   #define BUILD_DEBUG 0
>> svnrev.h echo #endif
>> svnrev.h echo.
>> svnrev.h echo #ifdef _DEBUG
>> svnrev.h echo   #define BUILD_STR "DEBUG"
>> svnrev.h echo #else
>> svnrev.h echo   #define BUILD_STR "RELEASE"
>> svnrev.h echo #endif
>> svnrev.h echo.
>> svnrev.h echo #include "starcraftver.h"
) else (
echo svnrev.h already exists, nothing to do here
pause
)