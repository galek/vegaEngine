#include "stdafx.h"
#include <wx/event.h>
#include "eventHandler_t.h"


namespace vega
{
	bool eventHandler_t::onEvent(wxEvent & event){
		return ProcessEvent(event);
	}
}