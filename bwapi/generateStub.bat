@echo off
if not exist svnrev.h (
echo #define SVN_REV 1111 >> svnrev.h
echo #define SVN_REV_STR "1111" >> svnrev.h
) else (
echo svnrev.h already exists, nothing to do here
pause
)