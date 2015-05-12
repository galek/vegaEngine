#pragma once
#include <wx/propgrid/propgrid.h>
class wxPropertyGridManager;

namespace vega
{
	class lightProp_t :public wxPanel, public moveObjectProp_t{
		DECLARE_EVENT_TABLE()
		Ogre::Light::LightTypes lightTypes;
		Ogre::Light *light;
		wxPropertyGridManager* propGrid;
	public:
		lightProp_t();
		bool init();
		virtual void update(Ogre::MovableObject *o, bool to);
		void OnPropertyGridChange(wxPropertyGridEvent& event);
		virtual bool Show(bool show = true);
	};
}