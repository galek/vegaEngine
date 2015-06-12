#pragma once
#include <wx/propgrid/propgrid.h>
class wxPropertyGridManager;

namespace vega
{
	class LightPropertyEditor :
		public wxPanel,
		public moveObjectProp_t,
		public Updatable
	{
		DECLARE_EVENT_TABLE()
		ActorLight::LightTypes lightTypes;
		ActorLight *light;
	public:
		LightPropertyEditor();
		~LightPropertyEditor();
		bool init();
		virtual void update(float t);
		/*DEPRECATED*/
		virtual void update(Ogre::MovableObject *o, bool to) {}
		void OnPropertyGridChange(wxPropertyGridEvent& event);
		virtual bool Show(bool show = true);
	private:
		virtual void _update(ActorLight *_obj);
	private:
		wxBoxSizer* m_TopSizer;
		wxPropertyGridManager* m_PropGrid;
	};
}