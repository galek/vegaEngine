/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef __PLATFORM_H_
#define __PLATFORM_H_

// Initial platform/compiler-related stuff to set.
#define ENGINE_PLATFORM_WINDOWS 1
#define ENGINE_PLATFORM_LINUX	2
#define ENGINE_PLATFORM_MAC     3
#define ENGINE_PLATFORM_ANDROID 4

#define ENGINE_COMPILER_MSVC    1
#define ENGINE_COMPILER_GNUC    2
#define ENGINE_COMPILER_ICC     3
#define ENGINE_COMPILER_CLANG   4


/* Finds the compiler type and version.
*/
#if defined( _MSC_VER ) && !defined (__ANDROID__)
#   define ENGINE_COMPILER ENGINE_COMPILER_MSVC
#   define ENGINE_COMP_VER _MSC_VER

#elif defined( _MSC_VER ) && defined (__ANDROID__)
#   define ENGINE_COMPILER ENGINE_COMPILER_GNUC
#   define ENGINE_COMP_VER _MSC_VER

#elif defined( __GNUC__ )
#   define ENGINE_COMPILER ENGINE_COMPILER_GNUC
#   define ENGINE_COMP_VER (((__GNUC__)*100)+__GNUC_MINOR__)

#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* See if we can use __forceinline or if we need to use __inline instead */
#if ENGINE_COMPILER == ENGINE_COMPILER_MSVC 
#       define EFORCEINLINE __forceinline
#else
#   define EFORCEINLINE __inline
#endif

/* Finds the current platform */

#if defined( __WIN32__ ) || defined(_WIN32) &&!defined(__ANDROID__)
#   define ENGINE_PLATFORM ENGINE_PLATFORM_WINDOWS

#elif defined( __APPLE_CC__)
#   define ENGINE_PLATFORM ENGINE_PLATFORM_MAC
#elif  defined( __ANDROID__)
#   define ENGINE_PLATFORM ENGINE_PLATFORM_ANDROID
#else
#   define ENGINE_PLATFORM ENGINE_PLATFORM_LINUX
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define ENGINE_QUOTE_INPLACE(x) # x
#define ENGINE_QUOTE(x) ENGINE_QUOTE_INPLACE(x)
#define ENGINE_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

//----------------------------------------------------------------------------
// Windows Settings
#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#include "PlatformWinDefines.h"
#endif
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Linux/Apple Settings
#if ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX || ENGINE_PLATFORM == ENGINE_PLATFORM_MAC


#endif

//For apple, we always have a custom config.h file
#if ENGINE_PLATFORM == ENGINE_PLATFORM_MAC
#endif

//----------------------------------------------------------------------------

#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#include "PlatformAndroidDefines.h"
#endif

#endif
