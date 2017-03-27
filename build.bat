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

pushd %CD%

if defined APPVEYOR (
  set MSBUILD_ADDITIONAL_OPTIONS=/logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
  set VSTEST_ADDITIONAL_OPTIONS=/logger:Appveyor
)

:: Build BWAPI's full stack
cd bwapi
::nuget restore
msbuild %MSBUILD_ADDITIONAL_OPTIONS% /verbosity:normal /p:Configuration=Debug_Pipeline bwapi.sln
msbuild %MSBUILD_ADDITIONAL_OPTIONS% /verbosity:normal /p:Configuration=Release_Pipeline bwapi.sln
msbuild %MSBUILD_ADDITIONAL_OPTIONS% /verbosity:normal /p:Configuration=Installer_Target bwapi.sln

:: Run unit tests
cd Debug
vstest.console BWAPILIBTest.dll BWAPICoreTest.dll %VSTEST_ADDITIONAL_OPTIONS%

:: Finish
popd

