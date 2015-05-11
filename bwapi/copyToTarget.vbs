'option explicit

Set objShell = CreateObject("WScript.Shell")
Set objFS = CreateObject("Scripting.FileSystemObject")
dim targetPath, sourceFile

on error resume next

if WScript.Arguments.Count = 0 then
  WScript.Echo("Expected source file as argument.")
  WScript.Quit(1)
end if

sourceFile = WScript.Arguments(0)
if not objFS.FileExists(sourceFile) then
  WScript.Echo("Source file does not exist.")
  WScript.Quit(2)
end if

targetPath = objShell.RegRead("HKCU\SOFTWARE\Blizzard Entertainment\Starcraft\InstallPath")
if not objFS.FolderExists(targetPath) then
  targetPath = objShell.RegRead("HKLM\SOFTWARE\Blizzard Entertainment\Starcraft\InstallPath")
  if not objFS.FolderExists(targetPath) then
    WScript.Echo("Failed to find Starcraft directory.")
    WScript.Quit(3)
  end if
end if

targetPath = targetPath & "\bwapi-data\"

if not objFS.FolderExists(targetPath) then
  WScript.Echo("Failed to locate bwapi-data folder.")
  WScript.Quit(4)
end if

WScript.Echo("Copying " & sourceFile & " to " & targetPath)
objFS.CopyFile sourceFile, targetPath, True
