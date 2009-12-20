@echo off
cd "..\Interface Packer"
del include\BWAPI2\*.* /q
del include\BWAPI\*.* /q
"..\debug\interface packer.exe" "BWAPI1.ini"
"..\debug\interface packer.exe" "BWAPI2.ini"
rem @pause