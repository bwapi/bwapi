@echo off
SubWCRev.exe . svnrev_template.h svnrev.h
if ERRORLEVEL 1 (
rem failure
echo ----------------------------
echo  SubWCRev.exe failed 
if not exist svnrev.h (
echo  creating a stub svnrev.h
call generateStub
) else (
echo  leaving svnrev.h untouched
)
echo ----------------------------
)
pause
