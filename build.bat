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

:: Retrieve dependencies
"apps/install-dependencies.bat"
set PATH=%PATH%;%CD%/apps;%CD%/apps/phantomjs;%CD%/apps/InnoSetup;%CD%/apps/cygwin/bin

pushd %CD%
cd bwapi
nuget restore
msbuild /verbosity:normal /target:Release_Pipeline;Debug_Pipeline bwapi.sln
msbuild /verbosity:normal /target:Installer_Target bwapi.sln
popd