/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once
/// @file

/** @mainpage
 *
 *  %Vega Engine is a complete game development framework for PCs,Androids
 *  providing a vast array of core technologies, content creation tools, and support infrastructure content.
 *
 */

//-----------------------------------------------------------------------------
// Standard includes.
//-----------------------------------------------------------------------------
#include <string>
#include <math.h> // M_PI, atan2()
#include <vector>
#ifdef OSX
#include <pwd.h> // getpwuid()
#elif LINUX
#include <pwd.h> // getpwuid()
#include <unistd.h> // getuid()
#include <sys/types.h> // getuid() getpwuid()
#endif
//-----------------------------------------------------------------------------
// Threading includes.
//-----------------------------------------------------------------------------
#include "..\..\external\ogre\include\OgreBuildSettings.h"
#ifdef USE_BOOST 
#include <boost/thread/thread.hpp>
#define BOOST_ASIO_NO_WIN32_LEAN_AND_MEAN
#include <boost/asio.hpp>
#else
//Threads
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#endif
//-----------------------------------------------------------------------------
// Platform-specifics
//-----------------------------------------------------------------------------
#include "..\platform\Platform.h"

//-----------------------------------------------------------------------------
// InterFaces
//-----------------------------------------------------------------------------
#include "ogreincs.h"
#include "BaseActorInterface.h"
#include "iPhysics.h"
#include "iPhysicsBodys.h"
#include "iAudioDevice.h"
#include "AudioUsefulFuncs.h"
#include "iGame.h"
#include "iFileSystem.h"
#include "SubSystemsManager.h"
#include "VVector.h"