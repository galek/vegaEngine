#pragma once

class wxListCtrl;
#include "wx/listbase.h"


namespace vega
{
	class blankProp_t :public wxWindow, public moveObjectProp_t{
	public:
		blankProp_t();
		bool init();
		virtual void update(Ogre::MovableObject *o, bool to);
	};
}