::FOR /R %%i in (*.cpp) DO sed "s/for (std::\([a-z]*\)<\([a-zA-Z\->.*]*\)>::iterator \([a-zA-Z]\) = \([a-zA-Z>.-]*\)\.begin();[ \n]*i[ ]*!=[ ]*\4\.end(); ++\3)/for each (\2 \3 in \4)/g" "%%i" > cpp.temp && type cpp.temp > "%%i" && del cpp.temp 
::FOR /R %%i in (*.cpp) DO sed "s/(\*\([a-z]\))/\1/g" "%%i" > cpp.temp && type cpp.temp > "%%i" && del cpp.temp
FOR /R %%i in (*.cpp) DO flip -u "%%i"