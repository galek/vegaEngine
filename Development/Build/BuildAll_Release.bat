@echo off
call BuildExternals_Release.bat
call BuildEngine_Release.bat
call BuildExternals_Release.x64.bat
call BuildEngine_Release.x64.bat