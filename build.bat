:: This file contains the minimum number of statements to create a full BWAPI release.
:: Testing and static analysis are external operations.

:: These operations require the following tools in the PATH environment variable:
::  - msbuild (Building the solution)
::  - git (Generating git information)
::  - doxygen (Documentation generation)
::    - graphvis/dot (Graphs in documentation)
::    - bash (Documentation post-processing (TODO: Switch out for BAT))
::    - pngcrush (image compression for better web experience)
::    - java (for HTML/CSS compression apps)
::  - Inno Setup (Installer)
::  - 7-Zip (archive without installer)

set PATH=%PATH%;%cd%/apps/doxygen/;%cd%/apps/graphviz/bin/;%cd%/apps/

if defined APPVEYOR (
  set MSBUILD_ADDITIONAL_OPTIONS=/logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
  set VSTEST_ADDITIONAL_OPTIONS=/logger:Appveyor
)

:: Prep repository
git submodule init
git submodule update

pushd bwapi
:: Restore nuget packages
curl --output nuget.exe https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
if %errorlevel% neq 0 exit 1

nuget restore -DisableParallelProcessing -verbosity detailed
if %errorlevel% neq 0 (
  timeout /t 2 /nobreak
  nuget restore -DisableParallelProcessing -verbosity detailed
  if %errorlevel% neq 0 exit 1
)

:: Build BWAPI's full stack
msbuild %MSBUILD_ADDITIONAL_OPTIONS% /verbosity:normal /p:Configuration=Debug /p:Platform=Win32 bwapi.sln
if %errorlevel% neq 0 exit 1

msbuild %MSBUILD_ADDITIONAL_OPTIONS% /verbosity:normal /p:Configuration=Release /p:Platform=Win32 bwapi.sln
if %errorlevel% neq 0 exit 1
::msbuild %MSBUILD_ADDITIONAL_OPTIONS% /verbosity:normal /p:Configuration=Installer_Target /p:Platform=Win32 bwapi.sln
popd

:: Documentation generation
set DOT_PATH=%cd%/apps/graphviz/bin/dot.exe
pushd Documentation

pip install -r requirements.txt
if %errorlevel% neq 0 exit 1

python m.css/documentation/doxygen.py Doxyfile-mcss
if %errorlevel% neq 0 exit 1

popd

:: Remove temp xml directory
rmdir /S /Q Release_Binary\documentation\xml


:: Archive artifacts
7z a -r -mx=9 -myx=9 BWAPI.7z Release_Binary
if %errorlevel% neq 0 exit 1

:: Finish
