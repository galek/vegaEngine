#ifndef VMaxLog_H__
#define VMaxLog_H__

#include <string>

namespace VMax
{

#define VMaxLog ::VMax::VMaxLogFile::Log

	class VMaxLogFile
	{
	public:
		// Sets the full path to the log file.
		static void SetPath(const std::string& logPath);
		// Returns the full path to the log file.
		static std::string GetPath(void);

		// Printf-style log output.
		static void Log(const char* format, ...);

		void operator<<(const char* str);
	protected:
		// The full path to the log file.
		static std::string _logPath;
	};

} // namespace 

#endif
