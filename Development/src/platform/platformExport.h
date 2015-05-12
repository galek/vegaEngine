/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#ifdef _DLIB
#ifdef PLATFORM_EXPORT
#define PLEXPORT __declspec(dllexport)
#else
#define PLEXPORT _declspec(dllimport)
#endif
#else
#define PLEXPORT 
#endif