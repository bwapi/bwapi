@echo off
cd "..\Interface Packer"
del include\BWAPI2\*.* /q
"..\debug\interface packer.exe" "BWSL.ini"
rem @pause