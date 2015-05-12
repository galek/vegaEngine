/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "RenderPrivate.h"
#ifdef USE_BOOST
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