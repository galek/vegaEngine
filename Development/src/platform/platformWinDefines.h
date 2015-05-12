/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef PLATFORM_WIN_DEFINES_H
#define PLATFORM_WIN_DEFINES_H


#ifdef ENGINE_PLATFORM_WINDOWS
#pragma warning (disable : 4067)
#pragma warning (disable : 4267)
#pragma warning (disable : 4267)
#pragma warning (disable : 4018)
#pragma warning (disable : 4305)
#pragma warning (disable : 4251)
#pragma warning (disable : 4275)
#pragma warning (disable : 4100)
#pragma warning (disable : 4512)
#pragma warning (disable : 4193)
#pragma warning (disable : 4049)//что импортированы локальные символы
#pragma warning (disable : 4275)//dll интерфейсы огра
#pragma warning (disable : 4099)//disable for havok "pdb not exist" warnings
#endif


#define stricmp _stricmp

#endif