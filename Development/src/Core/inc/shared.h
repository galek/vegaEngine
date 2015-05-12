/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once
//-----------------------------------------------------------------------------
// Visual Studio Warnings
//-----------------------------------------------------------------------------
#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#pragma warning(disable: 4231) /// Disable warnings on extern before template instantiation
#pragma warning(disable: 4786) /// STL 255 trunctuation
#pragma warning(disable: 4800) /// Forcing int to bool
#pragma warning(disable: 4018) /// Signed/Unsigned mismatch 
#pragma warning(disable: 4244) /// Double to float
#pragma warning(disable: 4267) /// size_t to int
#pragma warning(disable: 4251) /// class 'XXX' needs to have dll-interface to be used by its clients
#pragma warning(disable: 4311) /// Pointer truncation from 'LPCSTR' to 'DWORD'
#pragma warning(disable: 4312) /// conversion from 'DWORD' to 'LPCSTR' of greater size
#pragma warning(disable: 4251) /// needs to have dll-interface to be used by clients of class
#pragma warning(disable: 4305) /// Double -> F32
#pragma warning(disable: 4275) /// DLL-interface template export
#pragma warning(disable: 4100)
#pragma warning(disable: 4514)
#pragma warning(disable: 4996) /// 'XXX' was declared depreciated
#endif // #if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS

//-----------------------------------------------------------------------------
/// Unicode helpers
//-----------------------------------------------------------------------------

// !@Convert TypeName to Unicode String
template <typename T> std::wstring ToStrW(T a_T)
{  /// Convert any type to a string
	std::wostringstream buffer;
	buffer << a_T;
	return buffer.str();
}
// !@Convert TypeName to Ansi String
template <typename T> std::string ToStr(T a_T)
{  /// Convert any type to a string
	std::ostringstream buffer;
	buffer << a_T;
	return buffer.str();
}