#ifndef __Custom_Config_H_
#include "OgreBuildSettings.h"
#endif


#ifdef OGRE_USE_BOOST
#define BOOST_ALL_NO_LIB 1
#ifndef DEBUG
#pragma comment(lib,"libboost_date_time-vc120-1_55.lib")
#pragma comment(lib,"libboost_thread-vc120-mt-1_55.lib")
#pragma comment(lib,"libboost_system-vc120-1_55.lib")
#else
#pragma comment(lib,"libboost_date_time-vc120-gd-1_55.lib")
#pragma comment(lib,"libboost_thread-vc120-mt-gd-1_55.lib")
#pragma comment(lib,"libboost_system-vc120-mt-gd-1_55.lib")
#endif
#endif


#ifdef OGRE_STATIC_LIB
#pragma comment(lib,"FreeImage.lib")
#pragma comment(lib,"freetype249.lib")
#pragma comment(lib,"LibOpenJPEG.lib")
#pragma comment(lib,"LibPNG.lib")
#pragma comment(lib,"LibRaw.lib")
#pragma comment(lib,"LibTIFF4.lib")
#pragma comment(lib,"ZLib.lib")
#pragma comment(lib,"zziplib.lib")
#pragma comment(lib,"cg.lib")
#endif