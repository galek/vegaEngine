/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class ENGINE_API Input :public OIS::MouseListener, public OIS::KeyListener
	{
	public:
		enum InputFor
		{
			InputMODE_GAME = 0,
			InputMODE_GUIK = 1,
			InputMODE_GUIM = 2,
			InputMODE_GUIMK = 3,
		};
		InputFor InputMode, preventInputMode;
	public:
		Input();

		void frameRenderingQueued(void);
		void createFrameListener(void);
		void Recreate(bool);
		__inline bool GetIsBuffered() { return mBuffered; }
		bool keyPressed(const OIS::KeyEvent &arg);
		void windowResized(Ogre::RenderWindow* _window);
		// !@Unattach OIS before window shutdown (very important under Linux)
		void windowClosed(Ogre::RenderWindow* _window);
		bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id);

		bool mouseMoved( const OIS::MouseEvent &_arg );

		bool mouseReleased(const OIS::MouseEvent &_arg,OIS::MouseButtonID);
		bool keyReleased(const OIS::KeyEvent &_arg);

		void setInputMode(InputFor mode);
		void setInputModeHowPreventInputMode();
		void SetMouseLock(bool _status);
		__inline InputFor getInputMode(){ return InputMode; }
		__inline bool GetMouseLock(){ return mMouseLocked; }
		__inline bool GetKeyBoardLock(){ return mKeyBoardLocked; }
	protected:
		void CreateBuffered(void);
		void CreateUnBuffered(void);
	private:
		bool mInitialised,mBuffered;
		OIS::Keyboard*mKeyboard;
		OIS::Mouse*mMouse;
		OIS::InputManager*mInputManager;
		bool mMouseLocked, mKeyBoardLocked;
	};
}