/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/

#include "EnginePrivate.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	Input::Input()
		:mInputManager(nullptr)
		, mKeyboard(nullptr)
		, mMouse(nullptr)
		, InputMode(InputMODE_GAME)//Note:изначально режим для обычного инпута
		, mInitialised(false)
		, mBuffered(false)
	{
	}
	//-------------------------------------------------------------------------------------
	void Input::frameRenderingQueued() {
		if (mInitialised)
		{
			windowResized(GetEngine()->mGWindow);
			//Need to capture/update each device
			mKeyboard->capture();
			mMouse->capture();
		}
	}
	//-------------------------------------------------------------------------------------
	void Input::createFrameListener() {
		(mBuffered) ? CreateBuffered() : CreateUnBuffered();
	}
	//-------------------------------------------------------------------------------------
	void Input::windowResized(Ogre::RenderWindow* _window)	{
		unsigned int width, height, depth;
		int left, top;
		_window->getMetrics(width, height, depth, left, top);

		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = width;
		ms.height = height;
	}
	//-------------------------------------------------------------------------------------
	void Input::windowClosed(Ogre::RenderWindow* _window)
	{
		if (mInitialised)
		{
			//Only close for window that created OIS (the main window in these demos)
			if (_window == GetEngine()->mGWindow)
			{
				if (mInputManager)
				{
					mInputManager->destroyInputObject(mMouse);
					mInputManager->destroyInputObject(mKeyboard);

					OIS::InputManager::destroyInputSystem(mInputManager);
					mInputManager = 0;
				}
			}
			mInitialised = false;
		}
	}
	//-------------------------------------------------------------------------------------
	bool Input::keyPressed(const OIS::KeyEvent &_arg)
	{
		if (mInitialised)
		{
			switch (_arg.key) {
			case OIS::KC_SYSRQ:   // take a screenshot
				GetEngine()->mGWindow->writeContentsToTimestampedFile("UserData//screenshot", ".jpg");
				break;

				//Console
			case OIS::KC_GRAVE:
				bool status = !GetEngine()->GetConsole()->GetVisible();
				GetEngine()->GetConsole()->SetVisible(status);

				(status) ? setInputMode(InputMODE_GUIMK) : setInputModeHowPreventInputMode();//Если активна консоль включаем захват мыши и клавы,Иначе игровой режим
				break;
			}
			//Служебная функция,вот её я тебе настоятельно не рекомендую,ебался я долго с откладкой
			switch (InputMode){
			case InputMODE_GUIK:
			case InputMODE_GUIMK:
				MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(_arg.key), _arg.text);
				break;
			default:
				GetEngine()->GetSceneMgr()->InjectKeyDown(_arg.key);
			}
		}
		return true;
	}
	//-------------------------------------------------------------------------------------
	bool Input::keyReleased(const OIS::KeyEvent &_arg)
	{
		if (mInitialised)
		{
			//Служебная функция,вот её я тебе настоятельно не рекомендую,ебался я долго с откладкой
			switch (InputMode){
			case InputMODE_GUIK:
			case InputMODE_GUIMK:
				MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(_arg.key));
				break;
			default:
				GetEngine()->GetSceneMgr()->InjectKeyUp(_arg.key);
			}
		}
		return true;
	}
	//-------------------------------------------------------------------------------------
	bool Input::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
	{
		if (mInitialised)
		{
			switch (InputMode){
			case InputMODE_GUIMK:
			case InputMODE_GUIM:
				MyGUI::InputManager::getInstance().injectMousePress(evt.state.X.abs, evt.state.Y.abs, MyGUI::MouseButton::Enum(id));
				break;
			default:
				GetEngine()->GetSceneMgr()->InjectMouseDown(id);
			}
		}
		return true;
	}
	//-------------------------------------------------------------------------------------
	bool Input::mouseMoved(const OIS::MouseEvent &_arg)
	{
		if (mInitialised)
		{
			switch (InputMode){
			case InputMODE_GUIMK:
			case InputMODE_GUIM:
				MyGUI::InputManager::getInstance().injectMouseMove(_arg.state.X.abs, _arg.state.Y.abs, _arg.state.Z.abs);
				break;
			default:
				GetEngine()->GetSceneMgr()->InjectMouseMove(Ogre::Vector2(_arg.state.X.rel, _arg.state.Y.rel));
			}
		}
		return true;
	}
	//-------------------------------------------------------------------------------------
	bool Input::mouseReleased(const OIS::MouseEvent &_arg, OIS::MouseButtonID id)
	{
		if (mInitialised)
		{
			switch (InputMode){
			case InputMODE_GUIMK:
			case InputMODE_GUIK:
			case InputMODE_GUIM:
				MyGUI::InputManager::getInstance().injectMouseRelease(_arg.state.X.abs, _arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
				break;
			default:
				GetEngine()->GetSceneMgr()->InjectMouseUp(id);
			}
		}
		return true;
	}
	//-------------------------------------------------------------------------------------
	void Input::setInputMode(InputFor mode){
		preventInputMode = InputMode;
		InputMode = mode;
	}
	//-------------------------------------------------------------------------------------
	void Input::setInputModeHowPreventInputMode() {
		InputFor temp = InputMode;
		if (InputMode != preventInputMode){
			InputMode = preventInputMode;
			preventInputMode = temp;
		}
	}
	//-------------------------------------------------------------------------------------
	void Input::SetMouseLock(bool _status) {
		if (_status)
			mMouse->capture();
		mMouse->setBuffered(_status);
		mMouseLocked = _status;
	}
	//-------------------------------------------------------------------------------------
	void Input::CreateBuffered() {
		if (!mInitialised)
		{
			Debug("*** Initializing OIS Buffered***");
			OIS::ParamList pl;
			size_t windowHnd = 0;
			std::ostringstream windowHndStr;

			GetEngine()->mGWindow->getCustomAttribute("WINDOW", &windowHnd);
			windowHndStr << windowHnd;
			pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
			mInputManager = OIS::InputManager::createInputSystem(pl);

			mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
			mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

			mMouse->setEventCallback(this);
			mKeyboard->setEventCallback(this);
			mInitialised = true;
		}
	}
	//-------------------------------------------------------------------------------------
	void Input::CreateUnBuffered() {
		if (!mInitialised)
		{
			Debug("*** Initializing OIS nBuffered***");
			OIS::ParamList pl;
			size_t windowHnd = 0;
			std::ostringstream windowHndStr;

			GetEngine()->mGWindow->getCustomAttribute("WINDOW", &windowHnd);
			windowHndStr << windowHnd;
			pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
#if defined OIS_WIN32_PLATFORM
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
			pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
			pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
#endif

			mInputManager = OIS::InputManager::createInputSystem(pl);

			mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
			mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

			mMouse->setEventCallback(this);
			mKeyboard->setEventCallback(this);
			mInitialised = true;
		}
	}
	//-------------------------------------------------------------------------------------
	void Input::Recreate(bool _mBuffered){
		if (mInputManager)
		{
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
			mInitialised = false;
		}
		mBuffered = _mBuffered;
		createFrameListener();
	}
}