@echo off
call BuildExternals_Debug.bat
call BuildEngine_Debug.bat
call BuildExternals_Debug.x64.bat
call BuildEngine_Debug.x64.bat