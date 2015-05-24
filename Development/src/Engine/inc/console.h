/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#include <list>
#include <vector>
#include "GUI/BaseLayout/BaseLayout.h"

namespace vega
{
typedef MyGUI::delegates::CDelegate2<const MyGUI::UString&, const MyGUI::UString&> CommandDelegate;

	namespace formates
	{
		template<typename T> EFORCEINLINE std::string format()
		{
			return MyGUI::utility::toString("[ ", (std::numeric_limits<T>::min)(), " | ", (std::numeric_limits<T>::max)(), " ]");
		}
		template<> EFORCEINLINE std::string format<bool>()
		{
			return "[ true | false ]";
		}
		template<> EFORCEINLINE std::string format<float>()
		{
			return MyGUI::utility::toString("[ ", -(std::numeric_limits<float>::max)(), " | ", (std::numeric_limits<float>::max)(), " ]");
		}
		template<> EFORCEINLINE std::string format<double>()
		{
			return MyGUI::utility::toString("[ ", -(std::numeric_limits<double>::max)(), " | ", (std::numeric_limits<double>::max)(), " ]");
		}
	}

	class ENGINE_API Console :
		public wraps::BaseLayout,
		public Ogre::LogListener
	{
	public:
		Console();

		void PrintTextToConsole(const MyGUI::UString& _line);
		void PrintTextToConsole(const MyGUI::UString& _reason, const MyGUI::UString& _key, const MyGUI::UString& _value);

		void CleanConsole();
		void Initialize();

		/** Method : add command.\n
			@example Add_console_command
			@code
				RegisterConsoleDelegate("your_command_1","description", MyGUI::newDelegate(your_func));
				registerConsoleDelegate("your_command_2","description", MyGUI::newDelegate(your_static_method));
				registerConsoleDelegate("your_command_3","description", MyGUI::newDelegate(your_class_ptr, &your_class_name::your_method_name));
			@endcode

			signature your method : void method(const MyGUI::UString & _key, const MyGUI::UString & _value)
		*/
		void RegisterConsoleDelegate(const MyGUI::UString& _command,const MyGUI::UString& _des, CommandDelegate::IDelegate* _delegate);

		/** Remove command. */
		void UnregisterConsoleDelegate(const MyGUI::UString& _command);

		/** Event : Unknown command.\n
			signature : void method(const MyGUI::UString & _key, const MyGUI::UString & _value)
		*/
		CommandDelegate eventConsoleUnknowCommand;

		const MyGUI::UString& GetConsoleStringCurrent() const;
		const MyGUI::UString& GetConsoleStringError() const;
		const MyGUI::UString& GetConsoleStringSuccess() const;
		const MyGUI::UString& GetConsoleStringUnknow() const;
		const MyGUI::UString& GetConsoleStringFormat() const;

		bool GetVisible();
		void SetVisible(bool _visible);

		template <typename T>
		bool IsAction(T& _result, const MyGUI::UString& _key, const MyGUI::UString& _value, const MyGUI::UString& _format = "")
		{
			if (_value.empty())
			{
				addToConsole(getConsoleStringCurrent().c_str(), _key, MyGUI::utility::toString(_result).c_str());
			}
			else
			{
				if (!MyGUI::utility::parseComplex(_value, _result))
				{
					addToConsole(getConsoleStringError().c_str(), _key, _value);
					addToConsole(getConsoleStringFormat().c_str(), _key, _format.empty() ? formates::format<T>() : _format);
				}
				else
				{
					addToConsole(getConsoleStringSuccess().c_str(), _key, _value);
					return true;
				}
			}
			return false;
		}

		void GetListCommands();
	private:
		void NotifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button);

		void NotifyMouseButtonClick(MyGUI::Widget* _sender);
		void NotifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void NotifyButtonPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

		void RegisterBaseCommands();
		void messageLogged(const Ogre::String &, Ogre::LogMessageLevel, bool, const Ogre::String &, bool &);
	private:
		MyGUI::EditBox* mListHistory;
		MyGUI::ComboBox* mComboCommand;
		MyGUI::Button* mButtonSubmit;

		typedef std::map<MyGUI::UString, CommandDelegate> MapDelegate;
		MapDelegate mDelegates;
		typedef std::map<MyGUI::UString, MyGUI::UString> DescriptionsMap;
		DescriptionsMap mDescriptions;

		MyGUI::UString mStringCurrent;
		MyGUI::UString mStringError;
		MyGUI::UString mStringSuccess;
		MyGUI::UString mStringUnknow;
		MyGUI::UString mStringFormat;

		// если текущий текст автодополнен
		bool mAutocomleted;
		bool now, prev;
	};

};