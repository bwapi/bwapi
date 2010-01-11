@echo off
if not exist HTMLHelp_path.cmd (
echo set hhcpath=C:\Program Files\HTML Help Workshop>> HTMLHelp_path.cmd
)
call HTMLHelp_path.cmd
if not exist "%hhcpath%\hhc.exe" (
echo.
echo could not find hhc.exe in the specified path.
echo.
echo ---------------------------------------------------------------------
echo  please install HTMLHelp Workshop, It's free, download it from MSDN.
echo  Then if you choose a non-default installation directory, just put
echo  the installation path into the automatically generated
echo  HTMLHelp_path.cmd in this folder.
echo  Do not add the HTMLHelp_path.cmd to svn, it represents your local
echo  settings
echo ---------------------------------------------------------------------
echo.
pause
exit
)
"%hhcpath%\hhc" BWAPI_docs.hhp
pause
