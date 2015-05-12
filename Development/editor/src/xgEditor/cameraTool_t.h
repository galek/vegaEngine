#pragma once

#include "tool_t.h"
#include "update_t.h"

class wxTreebook;


namespace vega
{
	class frame_t;
	class cameraTool_t
		:public wxPanel, public tool_t, public update_t
	{
		wxPoint oldPoint;
		float rotVel;
		float moveVel;
		float xo;
		float yo;
		DECLARE_EVENT_TABLE()
		void OnSliderEvent(wxCommandEvent& event);
	public:
		void updateCamRotation();
		virtual bool init();
		virtual void update(float t);
		virtual bool onViewMouseEvent(wxMouseEvent& event);

	};
}