@echo off
cd "..\Interface Packer"
del include\BWAPI\*.* /q
"..\debug\interface packer.exe" "BWAPI.ini"
rem @pause