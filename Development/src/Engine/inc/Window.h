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

	class ENGINE_API Window:public Ogre::WindowEventListener
	{
	public:
		~Window();
		// Ogre::WindowEventListener
		void Listener();
	};
}