@echo off
call "%VS120COMNTOOLS%vsvars32.bat" /useenv > nul
devenv.com /nologo /rebuild "Release|x64" ../src/Engine.sln
pause
