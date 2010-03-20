@rem This is for the lazy people :)

@call "%VS90COMNTOOLS%\vsvars32.bat"
echo %CD%
@cmake-gui -H"%CD%" -B"%CD%"