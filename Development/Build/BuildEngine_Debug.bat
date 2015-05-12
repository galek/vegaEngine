@echo off
call "%VS120COMNTOOLS%vsvars32.bat" /useenv > nul
devenv.com /nologo /build "Debug|Win32" ../src/Engine.sln
