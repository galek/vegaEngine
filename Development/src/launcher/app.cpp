/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "app.h"

int main(int nCmdShow, char** lpCmdLine)
{
	using namespace vega;
	GameApp app;//Engine start
	app.Go();

	return 0;
}


#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow){
	return main(nCmdShow,(char**)lpCmdLine);
}
#else if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
void android_main(struct android_app* state) {
	main(0, 0);
}
#endif