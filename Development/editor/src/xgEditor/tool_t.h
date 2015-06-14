#pragma once

class wxListbook;
class wxTreebook;

namespace vega
{
	class MainFrame;
	class tool_t
	{
	public:
		virtual bool onActive();
		virtual bool onDeactive();
		virtual bool init();
		virtual bool onViewMouseEvent(wxMouseEvent & event);
		virtual bool onViewKeyEvent(wxKeyEvent & event);
		virtual ~tool_t(){}
	};
}