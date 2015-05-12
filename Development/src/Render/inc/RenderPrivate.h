/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef RENDER_PRIVATE_H
#define RENDER_PRIVATE_H

//-----------------------------------------------------------------------------
// Visual Studio Debugs
//-----------------------------------------------------------------------------
//#pragma message("Compiling RenderPrivate\n") 
#pragma warning(disable: 4267) /// convert type1->type2
#pragma warning(disable: 4305) /// convert double->float

// Core.dll
#include "../../Core/inc/CorePrivate.h"

//-----------------------------------------------------------------------------
// Render Module
//-----------------------------------------------------------------------------
#include "renderdllex.h"
#include "render.h"

// Deferred Renderer
#include "TerrainMaterialGeneratorD.h"

//-----------------------------------------------------------------------------
// Actor Deps.
//-----------------------------------------------------------------------------
#include "Externals.h"
#include "WaterSystem.h"
#include "VideoPlayer.h"
//-----------------------------------------------------------------------------
// Automatic Link.
//-----------------------------------------------------------------------------
#pragma comment(lib,"Core.lib")
//-----------------------------------------------------------------------------
// Actor Deps.
//-----------------------------------------------------------------------------
#endif//RENDER_PRIVATE_H