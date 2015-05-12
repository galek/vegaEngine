#include "ExPrerequisites.h"
#include "iparamb2.h"

extern ClassDesc2* GetVMaxDesc(void);
extern HINSTANCE hInstance;

HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
   //needed for xml format
   setlocale(LC_ALL, "English");

   if(fdwReason == DLL_PROCESS_ATTACH)
   {
	   // Hang on to this DLL's instance handle.
      hInstance = hinstDLL;        
      DisableThreadLibraryCalls(hInstance);
   }
	return TRUE;
}

__declspec(dllexport) const TCHAR* LibDescription(void)
{
	return _T("VMax");
}

__declspec(dllexport) int LibNumberClasses(void)
{
	return 2;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	switch(i) 
	{
		case 0: return GetVMaxDesc();
		default: return 0;
	}
}

__declspec(dllexport) ULONG LibVersion(void)
{
	return VERSION_3DSMAX;
}

__declspec(dllexport) ULONG CanAutoDefer(void)
{
	return 1;
}

TCHAR* GetString(int id)
{
	static TCHAR buf[256];

	if(hInstance)
	{
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	}
	return NULL;
}
