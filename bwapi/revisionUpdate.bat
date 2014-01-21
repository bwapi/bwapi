@echo off

set cmd="git rev-list HEAD --count"
FOR /F %%i IN (' %cmd% ') DO SET X=%%i

> svnrev.h echo static const int SVN_REV = 2383 + %X%;
>> svnrev.h echo.
>> svnrev.h echo #ifdef _DEBUG
>> svnrev.h echo   #define BUILD_STR "DEBUG"
>> svnrev.h echo   #define BUILD_DEBUG 1
>> svnrev.h echo #else
>> svnrev.h echo   #define BUILD_STR "RELEASE"
>> svnrev.h echo   #define BUILD_DEBUG 0
>> svnrev.h echo #endif
>> svnrev.h echo.
>> svnrev.h echo #include "starcraftver.h"

pause
