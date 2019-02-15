@ECHO OFF
MKDIR build
MKDIR build/res
XCOPY .\res .\build\res /s /e
PUSHD build
cmake ..
POPD
@ECHO ON