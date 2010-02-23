@echo off
if not exist svnrev.h (
SubWCRev.exe . svnrev_template.h svnrev.h
pause
) else (
echo svnrev.h already exists, nothing to do here
pause
)
