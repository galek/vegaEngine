#include "AiPrivate.h"

#pragma comment(lib,"Common.lib")
#pragma comment(lib,"Core.lib")

#pragma comment(lib,"OgreMain.lib")
#pragma comment(lib,"OgreTerrain.lib")

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