@echo off

set reptoolName=%1
if "%1"=="" set reptoolName=Replay_Tool_Expected.exe

set replaysSrcDir=%2
if "%2"=="" set replaysSrcDir=.\Replays

set repoDir=%3
if "%3"=="" set repoDir=.\ExpectedReplayTraces

echo Replay Tool: %reptoolName%
echo Replay Source: %replaysSrcDir%
echo Replay Traces Dir: %repoDir%
echo Extracting replay traces ...

for /r %replaysSrcDir% %%f in (*.rep) do call :call_extractor "%%f"
goto :eof

:call_extractor
set repPath=%1
echo Extracting %repPath%
%reptoolName% -r %repPath% %repoDir%
exit /b