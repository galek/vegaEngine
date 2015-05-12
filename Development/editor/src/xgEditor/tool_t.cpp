#include "stdafx.h"
#include "tool_t.h"


namespace vega
{
	bool tool_t::onViewMouseEvent(wxMouseEvent & event)
	{
		return false;
	}
	bool tool_t::onViewKeyEvent(wxKeyEvent & event){
		return false;
	}

	bool tool_t::onActive()
	{
		return false;
	}

	bool tool_t::onDeactive()
	{
		return false;
	}

	bool tool_t::init(){
		return true;
	}
}