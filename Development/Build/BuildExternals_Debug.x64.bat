@echo off
call "%VS120COMNTOOLS%vsvars32.bat" /useenv > nul
devenv.com /nologo /build "Debug|x64" ../external/BuildScripts/Externals.sln
