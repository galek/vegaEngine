/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/

#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H

#ifndef USE_DLL_GAME
#define USE_DLL_GAME 1							//we will load game how dll,using LoadLibrary
#endif

#ifndef _DEVELOP
#define _DEVELOP	 1					       // DEVELOPER Build
#endif
//Внутренний билд,используется для демонстрации возможностей
#ifndef _PRIVATEBUILD
//#define _PRIVATEBUILD 1
#endif

/*-------------------------------------------Features-------------------------------------------------*/
#ifndef USE_SKYX
#define USE_SKYX 1
#endif

#ifndef COMPONENT_USE_FFMPEG
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
#define COMPONENT_USE_FFMPEG 1
#endif
#endif

#ifndef USE_STEAMWORKS
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
#define USE_STEAMWORKS 1
#endif
#endif


#ifndef USE_D3D9_RENDER
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
#define USE_D3D9_RENDER 1
#endif
#endif

#ifndef USE_D3D11_RENDER
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
//#define USE_D3D11_RENDER 1
#endif
#endif

#ifndef USE_OGL_RENDER
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
//#define USE_OGL_RENDER 1
#endif
#endif

#ifdef _DEBUG
#define _DEVELOP 1
#endif
/*-------------------------------------Основные настройки движка-------------------------------------*/
#endif//BUILDCONFIG_H