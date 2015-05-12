::@echo off
SETLOCAL EnableDelayedExpansion

::SET N=0
::for /R %%i in (.) do (
::     SET DIR=%%i
::	
     ::put anything here, for instance the following code add dir numbers.
::    SET /A N=!N!+1
::    echo !N! !DIR!
::)

for /r %%x in (*.mesh) do (  
    set di=%%x
    echo directory !di!
    "..\Binaries.win32\OgreMeshUpgrader.exe" "%%x"
)