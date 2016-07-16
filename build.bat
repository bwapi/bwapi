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

:: Build BWAPI's full stack
cd bwapi
nuget restore
msbuild /verbosity:normal /p:Configuration=Debug_Pipeline bwapi.sln
msbuild /verbosity:normal /p:Configuration=Release_Pipeline bwapi.sln
msbuild /verbosity:normal /p:Configuration=Installer_Target bwapi.sln

:: Run unit tests
cd Debug
if defined APPVEYOR (
  vstest.console BWAPILIBTest.dll BWAPICoreTest.dll /logger:Appveyor
) else (
  vstest.console BWAPILIBTest.dll BWAPICoreTest.dll
)
:: Finish
popd

