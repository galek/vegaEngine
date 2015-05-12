/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"

#pragma comment(lib,"OgreMain.lib")
#pragma comment(lib,"OIS_static.lib")

#pragma comment(lib,"Common.lib")
#pragma comment(lib,"Core.lib")
#pragma comment(lib,"Render.lib")


#pragma comment(lib,"platform.lib")


#pragma comment(lib,"OgreTerrain.lib")


#pragma comment(lib,"MyGUICommon.lib")
#pragma comment(lib,"MyGUI.OgrePlatform.lib")
#pragma comment(lib,"MyGUIEngine.lib")
#pragma comment(lib,"freetype249.lib")
#ifdef USE_BOOST
#define BOOST_ALL_NO_LIB 1
#ifndef _DEBUG
#pragma comment(lib,"libboost_date_time-vc120-1_55.lib")
#pragma comment(lib,"libboost_thread-vc120-mt-1_55.lib")
#pragma comment(lib,"libboost_system-vc120-1_55.lib")
#else
#pragma comment(lib,"libboost_date_time-vc120-gd-1_55.lib")
#pragma comment(lib,"libboost_thread-vc120-mt-gd-1_55.lib")
#pragma comment(lib,"libboost_system-vc120-mt-gd-1_55.lib")
#endif
#endif