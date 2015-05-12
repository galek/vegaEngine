#pragma once

#if 0
#if defined( _DEBUG )
#define alarm {\
static bool ignoreAlways = false; \
alarm_t::get().set(__LINE__, __FILE__,&ignoreAlways);}\
if(!alarm_t::get().getIgnoreAlways() )alarm_t::get().alarming
#else
#define alarm(...)
#endif
#include <string>


namespace vega
{
	class alarm_t
	{
		int line;
		const char *file;
		bool *ignoreAlways;
	public:
		alarm_t(void);
		bool getIgnoreAlways();
		void set(int line, const char* file, bool* ignoreAlways);
		void alarming(bool value, const char* format, ...);
		void alarming(bool value, const wchar_t* format, ...);
		void alarming(bool value, const std::string& desc);
		bool alarming(bool value, const wchar_t* description, int line, const char* file, bool* ignoreAlways);
		bool alarming(bool value, const char* description, int line, const char* file, bool* ignoreAlways);
		bool alarming(bool value, const std::string& description, int line, const char* file, bool* ignoreAlways);
		static alarm_t& get();
		virtual ~alarm_t(void);
	};
}
#endif