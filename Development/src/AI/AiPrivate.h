#pragma once

// STL:
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#include <windows.h>
#endif
#include <vector>

// recast/detour stuff
#include "libs/Detour/DetourCommon.h"
#include "libs/DetourCrowd/DetourCrowd.h"
#include "libs/Recast/Recast.h"
#include "libs/Detour/DetourNavMesh.h"
#include "libs/Detour/DetourNavMeshBuilder.h"
#include "libs/Detour/DetourNavMeshQuery.h"
#include "libs/Detour/DetourNavMeshQuery.h"
#include "libs/RecastContrib/fastlz/fastlz.h"

// Engine Interfaces
#include "../../Common/inc/IEngine.h"
// Core.dll
#include "../../Core/inc/CorePrivate.h"

// This Module
#include "AIdefs.h"
#include "OgreRecastDefinitions.h"
#include "ogrerecast.h"
#include "OgreDetourCrowd.h"