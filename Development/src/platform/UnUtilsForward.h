#pragma once

int appStrlen(const wchar_t* String);
int appStrlen(const char* String);
const wchar_t* appBaseDirWCSTR();
const char* appBaseDirCSTR();

EFORCEINLINE const TCHAR* appBaseDir()
{
#ifdef _UNICODE
	return appBaseDirWCSTR();
#else
	return appBaseDirCSTR();
#endif
}