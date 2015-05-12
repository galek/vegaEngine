#pragma once

#include "tool_t.h"

namespace vega
{
	class frame_t;
	//##ModelId=44B9D7F0000F
	class renderOptionTool_t :public wxPanel, public tool_t
	{
		DECLARE_EVENT_TABLE()
	public:
		//##ModelId=44BA0692038A
		void onButtonEvent(wxCommandEvent &event);
		//##ModelId=44BA0693002E
		renderOptionTool_t();
	};
}