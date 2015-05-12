#pragma once

namespace vega
{
	class eventHandler_t :public wxEvtHandler{
	public:
		virtual bool onEvent(wxEvent & event);
	};
}