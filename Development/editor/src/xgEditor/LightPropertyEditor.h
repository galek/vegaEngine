#pragma once

#include <wx/propgrid/propgrid.h>
class wxPropertyGridManager;

#include "moveObjectProp_t.h"

namespace vega
{
	class LightPropertyEditor :
		public wxPanel,
		public moveObjectProp_t,
		public Updatable
	{
		DECLARE_EVENT_TABLE()
		ActorLight::LightTypes lightTypes;
		//TODO:Сделать вектором,что бы можно было редактировать списком
		ActorLight *m_EditableLight;
	public:
		LightPropertyEditor();
		~LightPropertyEditor();
		bool init();
		virtual void update(float t);
		/*DEPRECATED*/
		virtual void update(Ogre::MovableObject *o, bool to) {}
		void OnPropertyGridChange(wxPropertyGridEvent& event);
		virtual bool Show(bool show = true);
		void SetEditableLight(ActorLight* _mEditableLight);
		void SetEditableLightIsNull();
	private:
		virtual void _update();
	private:
		wxBoxSizer* m_TopSizer;
		wxPropertyGridManager* m_PropGrid;
	};
}