/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#pragma message("NICK-REFACTOR IT 2015")
//-----------------------------------------------------------------------------
CORE_API void Warning(const char *fmt, ...)
{
	va_list mark;
	char buf[1024];
	va_start(mark, fmt);
	int sz = vsprintf(buf, fmt, mark); buf[sizeof(buf) - 1] = 0;
	va_end(mark);
	if (sz)
	{
		std::string sMsg = "[WARNING]: ";
		sMsg += buf;
		LogPrintf(sMsg.c_str());
	}
}

//-----------------------------------------------------------------------------
CORE_API void LogPrintf(const char *fmt, ...)
{
	va_list mark;
	char buf[1024];
	va_start(mark, fmt);
	int sz = vsprintf(buf, fmt, mark); buf[sizeof(buf) - 1] = 0;
	va_end(mark);
	if (sz)
	{
		auto ptr = Ogre::LogManager::getSingletonPtr();
		if (ptr)
			ptr->logMessage(buf);
		else
			printf(buf);
	}
}

//-----------------------------------------------------------------------------
CORE_API void SeriousWarning(bool _show, const char *fmt, ...)
{
	va_list mark;
	char buf[1024];
	va_start(mark, fmt);
	int sz = vsprintf(buf, fmt, mark); buf[sizeof(buf) - 1] = 0;
	va_end(mark);
	if (sz)
	{
		std::string str = ("[Serious Warning] %s\n", buf);
		LogPrintf(str.c_str());
		if (_show)
		{
#if WIN32
			MessageBox(0, buf, "[Serious Warning]", 0);
#else
			Ogre::ErrorDialog* dlg = new Ogre::ErrorDialog();

			dlg->display(str.c_str());
			delete dlg;
#endif
		}
	}
}

//-----------------------------------------------------------------------------
CORE_API void Debug(const char *fmt, ...)
{
#ifdef _DEVELOP
	va_list mark;
	char buf[1024];
	va_start(mark, fmt);
	int sz = vsprintf(buf, fmt, mark); buf[sizeof(buf) - 1] = 0;
	va_end(mark);
	if (sz)
	{
		std::string sMsg = "[DEBUG]: ";
		sMsg += buf;
		LogPrintf(sMsg.c_str());
	}
#endif

}

//-----------------------------------------------------------------------------
CORE_API void SetLoggingLevel(int _level)
{
	auto ptr = Ogre::LogManager::getSingletonPtr();
	if (!ptr)
		return;
	ptr->setLogDetail((Ogre::LoggingLevel)_level);
}

//-----------------------------------------------------------------------------
CORE_API int GetLoggingLevel()
{
	auto ptr = Ogre::LogManager::getSingletonPtr();
	if (!ptr)
		return 0;
	return ptr->getDefaultLog()->getLogDetail();
}