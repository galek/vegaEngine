@echo off
call "%VS120COMNTOOLS%vsvars32.bat" /useenv > nul
devenv.com /nologo /build "Release|x64" ../external/BuildScripts/Externals.sln
