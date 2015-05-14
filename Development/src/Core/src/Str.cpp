/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"
#pragma message("NICK-REFACTOR IT 2015")

//-------------------------------------------------------------------------------------
//Вывод целого числа в виде строки
std::string ToStr(const char *fmt, ...)
{
	static char		msg[8000];

	va_list		argptr;
	va_start (argptr,fmt);
	vsprintf (msg,fmt,argptr);
	va_end (argptr);

	return msg;
}