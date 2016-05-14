pushd %CD%
cd /D %~dp0
cygwin --quiet-mode --no-shortcuts --prune-install --no-admin --root ./cygwin/ --packages git cppcheck doxygen graphviz pngcrush
popd