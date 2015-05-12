#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define AI_API 
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef AI_EXPORTS
#define AI_API __declspec(dllexport)
#else
#define AI_API __declspec(dllimport)
#endif
#endif