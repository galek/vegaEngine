#pragma once
#include <wx/propgrid/propgrid.h>
class wxPropertyGridManager;

namespace vega
{
	class lightProp_t :
		public wxPanel, 
		public moveObjectProp_t,
		public Updatable
	{
		DECLARE_EVENT_TABLE()
		Ogre::Light::LightTypes lightTypes;
		Ogre::Light *light;
	public:
		lightProp_t();
		~lightProp_t();
		bool init();
		virtual void update(float t);
		/*DEPRECATED*/
		virtual void update(Ogre::MovableObject *o, bool to);
		void OnPropertyGridChange(wxPropertyGridEvent& event);
		virtual bool Show(bool show = true);
	private:
		virtual void _update(Ogre::Light *_obj);
	private:
		wxBoxSizer* topSizer;
		wxPropertyGridManager* propGrid;
	};
}