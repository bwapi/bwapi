@rem This is for the lazy people :)

@call "%VS90COMNTOOLS%\vsvars32.bat"
@cmake-gui -H"%CD%" -B"%CD%"