Set objShell = CreateObject("WScript.Shell")
Set objFS = CreateObject("Scripting.FileSystemObject")

revNumber = 0

hasGitResult = objShell.Run("where git", 0, True)
If hasGitResult = 0 Then
  Set gitResult = objShell.Exec("git rev-list HEAD --count")
  ' 2383 is the number of revisions that were cut when migrating to Github
  ' Adding this value makes it more consistent with older releases
  revNumber = 2383 + CInt(gitResult.StdOut.ReadAll())
End If

Set svnrev = objFS.CreateTextFile("svnrev.h")
svnrev.WriteLine("static const int SVN_REV = " & revNumber & ";")
svnrev.WriteLine("")
svnrev.WriteLine("#include ""starcraftver.h""")
svnrev.WriteLine("")
svnrev.Close()
