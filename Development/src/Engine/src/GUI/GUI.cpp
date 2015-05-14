/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "GUI/GUI.h"


namespace vega
{
	GUI::GUI()
		:mPlatform(nullptr)
		, mGUI(nullptr)
	{
		//GUI package
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..\\Engine\\Content\\GUIBase.npk","Zip",Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,true);
	}
	/*
	*/
	void GUI::Initialize()
	{
		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(GetEngine()->mGWindow, GetEngine()->mGSceneMgr, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
		mGUI = new MyGUI::Gui();
		mGUI->initialise("MyGUI_Core.xml");
		//Default States
		ShowGUIMouseCursor(false);
	}
	/*
	*/
	bool GUI::IsShowingGUIMouseCursor()	{
		return MyGUI::PointerManager::getInstance().isVisible();
	}
	/*
	*/
	void GUI::ShowGUIMouseCursor(bool _evt)	{
		MyGUI::PointerManager::getInstance().setVisible(_evt);
	}
}