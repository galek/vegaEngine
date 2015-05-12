#pragma once

#include "tool_t.h"

namespace vega
{
	class frame_t;

	class testTool_t : public tool_t
	{
	public:

		void onButtonEvent(wxCommandEvent &event);

		void onTextEvent(wxCommandEvent &event);

		testTool_t();
	private:
		DECLARE_EVENT_TABLE()
	};
}