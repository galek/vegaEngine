/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "platform.h"

#include "platformWinDefines.h"


inline int appStrlen(const wchar_t* String) { return wcslen(String); }
inline int appStrlen(const char* String) { return strlen(String); }

const char* appBaseDirCSTR()
{
	static TCHAR Result[256] = _T("");
	if (!Result[0])
	{
		// Get directory this executable was launched from.
		{
			GetModuleFileName(NULL, Result, ARRAY_COUNT(Result));
		}
		int i;
		for (i = appStrlen(Result) - 1; i>0; i--)
			if (Result[i - 1] == PATH_SEPARATOR[0] || Result[i - 1] == '/')
			break;
		Result[i] = 0;
	}
	return Result;
}

const wchar_t * appBaseDirWCSTR()
{
	static wchar_t  Result[256] = L"";
	if (!Result[0])
	{
		// Get directory this executable was launched from.
		{
			GetModuleFileNameW(NULL, Result, ARRAY_COUNT(Result));
		}
		int i;
		for (i = appStrlen(Result) - 1; i>0; i--)
			if (Result[i - 1] == PATH_SEPARATOR[0] || Result[i - 1] == '/')
			break;
		Result[i] = 0;
	}
	return Result;
}