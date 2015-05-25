/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "Console.h"
#include "ConsoleCommands.h"
#include "APIEngineCommands.h"

using namespace Ogre;
using namespace std;

namespace vega
{
	/**
	*/
	Console::Console() : BaseLayout(), now(false), prev(false)	{	}
	/**
	*/
	void Console::Initialize()
	{
		BaseLayout::initialise("Console.layout");
		assignWidget(mListHistory, "list_History");
		assignWidget(mComboCommand, "combo_Command");
		assignWidget(mButtonSubmit, "button_Submit");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr) window->eventWindowButtonPressed += newDelegate(this, &Console::NotifyWindowButtonPressed);

		mStringCurrent = mMainWidget->getUserString("Current");
		mStringError = mMainWidget->getUserString("Error");
		mStringSuccess = mMainWidget->getUserString("Success");
		mStringUnknow = mMainWidget->getUserString("Unknown");
		mStringFormat = mMainWidget->getUserString("Format");

		mAutocomleted = false;

		mComboCommand->eventComboAccept += newDelegate(this, &Console::NotifyComboAccept);
		mComboCommand->eventKeyButtonPressed += newDelegate(this, &Console::NotifyButtonPressed);
		mButtonSubmit->eventMouseButtonClick += newDelegate(this, &Console::NotifyMouseButtonClick);
		mListHistory->setOverflowToTheLeft(true);

		SetVisible(false);
		RegisterBaseCommands();
		Ogre::LogManager::getSingletonPtr()->getDefaultLog()->addListener(this);
	}
	/**
	*/
	void Console::NotifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button) 	{
		if (_button == "close")
			mMainWidget->setVisible(false);
	}
	/**
	*/
	void Console::NotifyMouseButtonClick(MyGUI::Widget* _sender)	{
		NotifyComboAccept(mComboCommand, MyGUI::ITEM_NONE);
	}
	/**
	*/
	void Console::NotifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		const MyGUI::UString& command = _sender->getOnlyText();
		if (command == "") return;

		MyGUI::UString key = command;
		MyGUI::UString value;

		size_t pos = command.find(' ');
		if (pos != MyGUI::UString::npos)
		{
			key = command.substr(0, pos);
			value = command.substr(pos + 1);
		}

		MapDelegate::iterator iter = mDelegates.find(key);
		if (iter != mDelegates.end())
			iter->second(key, value);

		else
		{
			if (eventConsoleUnknowCommand.empty())
				PrintTextToConsole(mStringUnknow + "'" + key + "'");

			else
				eventConsoleUnknowCommand(key, value);

		}

		_sender->setCaption("");
	}
	/**
	*/
	void Console::NotifyButtonPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
	{
		MyGUI::EditBox* edit = _sender->castType<MyGUI::EditBox>();
		size_t len = edit->getCaption().length();
		if ((_key == MyGUI::KeyCode::Backspace) && (len > 0) && (mAutocomleted))
		{
			edit->deleteTextSelection();
			len = edit->getCaption().length();
			edit->eraseText(len - 1);
		}

		MyGUI::UString command = edit->getCaption();
		if (command.length() == 0)
			return;

		for (MapDelegate::iterator iter = mDelegates.begin(); iter != mDelegates.end(); ++iter)
		{
			if (iter->first.find(command) == 0)
			{
				if (command == iter->first) break;
				edit->setCaption(iter->first);
				edit->setTextSelection(command.length(), iter->first.length());
				mAutocomleted = true;
				return;
			}
		}
		mAutocomleted = false;
	}
	/**
	*/
	void Console::PrintTextToConsole(const MyGUI::UString& _line)
	{
		if (mListHistory->getCaption().empty())
			mListHistory->addText(_line);
		else
			mListHistory->addText("\n" + _line);

		mListHistory->setTextSelection(mListHistory->getTextLength(), mListHistory->getTextLength());
	}
	/**
	*/
	void Console::CleanConsole()	{
		mListHistory->setCaption("");
	}
	/**
	*/
	void Console::RegisterConsoleDelegate(const MyGUI::UString& _command, const MyGUI::UString& _des, CommandDelegate::IDelegate* _delegate)
	{
		mComboCommand->addItem(_command);
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
			MYGUI_LOG(Warning, "console - command '" << _command << "' already exist");

		mDelegates[_command] = _delegate;
		//Описания
		DescriptionsMap::iterator Desiter = mDescriptions.find(_command);
		if (Desiter != mDescriptions.end())
			MYGUI_LOG(Warning, "console - command '" << _command << "' already descripted");

		mDescriptions[_command] = _des;
	}

	void Console::UnregisterConsoleDelegate(const MyGUI::UString& _command)
	{
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			mDelegates.erase(iter);
			for (size_t i = 0; i < mComboCommand->getItemCount(); ++i)
			{
				if (mComboCommand->getItemNameAt(i) == _command)
				{
					mComboCommand->removeItemAt(i);
					break;
				}
			}
		}
		else
			MYGUI_LOG(Warning, "console - command '" << _command << "' doesn't exist");
	}
	/**
	*/
	void Console::PrintTextToConsole(const MyGUI::UString& _reason, const MyGUI::UString& _key,
		const MyGUI::UString& _value)	{
		PrintTextToConsole(MyGUI::utility::toString(_reason, "'", _key, " ", _value, "'"));
	}
	/**
	*/
	const MyGUI::UString& Console::GetConsoleStringCurrent() const	{
		return mStringCurrent;
	}
	/**
	*/
	const MyGUI::UString& Console::GetConsoleStringError() const	{
		return mStringError;
	}
	/**
	*/
	const MyGUI::UString& Console::GetConsoleStringSuccess() const	{
		return mStringSuccess;
	}
	/**
	*/
	const MyGUI::UString& Console::GetConsoleStringUnknow() const	{
		return mStringUnknow;
	}
	/**
	*/
	const MyGUI::UString& Console::GetConsoleStringFormat() const	{
		return mStringFormat;
	}
	/**
	*/
	bool Console::GetVisible()	{
		return mMainWidget->getVisible();
	}
	/**
	*/
	void Console::SetVisible(bool _visible)	{
#if 0
		prev = API::IsBufferredInput();
		now = !prev;
		mMainWidget->setVisible(_visible);
		if (prev != now)
			API::SetBufferedUnBufferedMouseMode(now, API::GetShowGUICursor());
		else
			API::SetBufferedUnBufferedMouseMode(prev, API::GetShowGUICursor());
#else
		now = _visible;
		mMainWidget->setVisible(_visible);
		if (prev != _visible)
			API::SetBufferedUnBufferedMouseMode(prev, API::GetShowGUICursor());
		else
			API::SetBufferedUnBufferedMouseMode(_visible, API::GetShowGUICursor());

		if (prev != API::IsBufferredInput())
			prev = now;
#endif

	}
	/**
	*/
	void Console::GetListCommands()
	{
		//Описания
		for (DescriptionsMap::iterator i = mDescriptions.begin(); i != mDescriptions.end(); i++)
			PrintTextToConsole(std::string(i->first + i->second));
	}
	/**
	*/
	void Console::RegisterBaseCommands()
	{
		RegisterConsoleDelegate("help", " -get list commands with description", MyGUI::newDelegate(&ConsoleCommands::Help));
		RegisterConsoleDelegate("clean", " -clean console", MyGUI::newDelegate(&ConsoleCommands::CleanConsole));
		RegisterConsoleDelegate("quit", " -engine shoutdown", MyGUI::newDelegate(&ConsoleCommands::EngineShoutdown));
		RegisterConsoleDelegate("loadlevel", " -loading new level", MyGUI::newDelegate(&ConsoleCommands::LoadLevel));
		RegisterConsoleDelegate("playv", " -play video clip", MyGUI::newDelegate(&ConsoleCommands::PlayV));
		RegisterConsoleDelegate("r_aa", " -render antialiasing", MyGUI::newDelegate(&ConsoleCommands::R_AA));
		RegisterConsoleDelegate("r_ssao", " -render SSAO", MyGUI::newDelegate(&ConsoleCommands::R_SSAO));
		RegisterConsoleDelegate("r_hdr", " -render hdr", MyGUI::newDelegate(&ConsoleCommands::R_HDR));
		RegisterConsoleDelegate("runscript", " -running script", MyGUI::newDelegate(&ConsoleCommands::RunScript));
		RegisterConsoleDelegate("pause", " -set pause", MyGUI::newDelegate(&ConsoleCommands::E_Pause));
		RegisterConsoleDelegate("reloadcfg", " -reload config", MyGUI::newDelegate(ConsoleCommands::ReloadCfg));
	}
	/**
	 */
	void Console::messageLogged(const Ogre::String &message, Ogre::LogMessageLevel, bool, const Ogre::String &, bool &){
		PrintTextToConsole("#FFE600" + message);
	}
}