@echo off
cd "..\Interface Packer"
del include\BWAPI2\*.* /q
"..\debug\interface packer.exe" "BWAPI2.ini"
rem @pause