/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
#include "MyGUI_Precompiled.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_FileLogListener.h"
#include "MyGUI_ConsoleLogListener.h"
#include "MyGUI_LevelLogFilter.h"
#include "MyGUI_LogSource.h"
#include <time.h>

#include "../../../../src/Core/inc/Log.h"

namespace MyGUI
{

	LogManager* LogManager::msInstance = nullptr;

	LogManager::LogManager() :
		mConsole(nullptr),
		mFile(nullptr),
		mFilter(nullptr),
		mDefaultSource(nullptr),
		mLevel(LogLevel::Info),
		mConsoleEnable(true)
	{
		msInstance = this;
	}

	LogManager::~LogManager()
	{
		msInstance = nullptr;
	}

	LogManager& LogManager::getInstance()
	{
		if (msInstance == nullptr)
		{
			MYGUI_DBG_BREAK;
			MYGUI_BASE_EXCEPT("Singleton instance LogManager was not created", "MyGUI");
		}
		return *msInstance;
	}

	LogManager* LogManager::getInstancePtr()
	{
		return msInstance;
	}

	void LogManager::flush()
	{}

	void LogManager::log(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line)
	{
		switch (_level.getValue())
		{
		case LogLevel::Info:
			::Debug((_section+_message).c_str(), _file, _line);
			break;
		case LogLevel::Warning:
			::Warning((_section + _message).c_str(), _file, _line);
			break;
		case LogLevel::Error:
			::SeriousWarning(false,(_section + _message).c_str(), _file, _line);
			break;
		case LogLevel::Critical:
			::ErrorFunction(true, (_section + _message).c_str(), _file, _line);
			break;
		}
	}

	void LogManager::close()
	{}

	void LogManager::addLogSource(LogSource* _source)
	{}

	void LogManager::createDefaultSource(const std::string& _logname)
	{}

	void LogManager::setSTDOutputEnabled(bool _value)
	{}

	bool LogManager::getSTDOutputEnabled() const
	{	return 0; }

	void LogManager::setLoggingLevel(LogLevel _value)
	{}

	LogLevel LogManager::getLoggingLevel() const
	{
		return mLevel;
	}

} // namespace MyGUI
