#ifndef _EXPREREQUISITES_H
#define _EXPREREQUISITES_H

#define PRECISION 0.000001

#if _MSC_VER <1600
#define nullptr 0
#endif

// Change this to the appropriate value to target Windows XP or later.
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x0501 

#include "max.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include <IGame/IGame.h>
#include <IGame/IGameModifier.h>
#include <IPathConfigMgr.h> 
#include <ilayer.h>

// This file is not included in the max SDK directly, but in the morpher sample.
// It is required to get access to the Max Morpher. 
#include <../samples/modifiers/morpher/wm3.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 4267)
#pragma warning (disable : 4018)

// OGRE API
// Max defines PI and OgreMath.h fails to compile as a result.
#undef PI 
// Max defines base_type and boost(which ogre uses) fails to compile as a result.
#undef base_type
#include "Ogre.h"

// This used to be contained in a file called OgreNoMemoryMacros.h, which was removed in version 1.6 of Ogre.
#ifdef OGRE_MEMORY_MACROS
#undef OGRE_MEMORY_MACROS
#undef new
#undef delete
#undef malloc
#undef calloc
#undef realloc
#undef free
#endif

#include "OgreDefaultHardwareBufferManager.h"
#define PI 3.1415926535f



// standard libraries
#include <math.h>
#include <vector>
#include <set>
#include <cassert>

extern TCHAR* GetString(int id);
extern HINSTANCE hInstance;

namespace VMax
{
  class ExOgreConverter;
  class ExMaterialSet;
  class ExMaterial;
  class ExBone;
  class ExVertex;
}

#include "..\tinyxml\tinyxml.h"
#include "..\..\..\src\Core\inc\shared.h"
#include "..\..\..\src\Core\inc\Core.h"
#endif
