@echo off
for %%X in (SubWCRev.exe) do (set SUBWCREVEXE=%%~$PATH:X)
if defined SUBWCREVEXE (
"%SUBWCREVEXE%" . svnrev_template.h svnrev.h
) else (
SubWCRev.fromSVN.exe . svnrev_template.h svnrev.h
)
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
