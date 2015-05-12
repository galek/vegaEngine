/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2015 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

//-----------------------------------------------------------------------------
CORE_API void ErrorFunction(bool _toLog, const char *fmt, const char *_file, int _line, ...)
{
	char		msg[1024];

	va_list		argptr;
	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	std::string str(fmt);
	str += "\n IN:";
	str += _file;
	str += std::to_string(_line);

	if (_toLog)
		LogPrintf(str.c_str());
#if WIN32
	MessageBox(0, str.c_str(), "[ERROR]", 0);
#else
	Ogre::ErrorDialog* dlg = new Ogre::ErrorDialog(); 

	dlg->display(str.c_str());
	delete dlg;
#endif
}