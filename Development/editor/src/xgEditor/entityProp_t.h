#pragma once
class wxListCtrl;
#include "wx/listbase.h"


namespace vega
{
	class entityProp_t :public wxPanel, public moveObjectProp_t{
		DECLARE_EVENT_TABLE()
		wxListCtrl *wlc;
		Ogre::Entity *e;
	public:
		entityProp_t();
		~entityProp_t();
		bool init();
		virtual void update(Ogre::MovableObject *o, bool to);
		virtual void update(float t);
		void onColClick(wxListEvent& event);
		void onRightClick(wxListEvent& event);
		virtual bool Show(bool show = true);
	};
}