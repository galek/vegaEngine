/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LogSource.h"

namespace MyGUI
{

	LogSource::LogSource() :
		mFilter(nullptr)
	{
	}

	LogSource::~LogSource()
	{
	}

	void LogSource::setLogFilter(ILogFilter* _filter)
	{
		mFilter = _filter;
	}

	void LogSource::addLogListener(ILogListener* _lestener)
	{}

	void LogSource::open()
	{}

	void LogSource::close()
	{}

	void LogSource::flush()
	{}

	void LogSource::log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
	{}

} // namespace MyGUI
