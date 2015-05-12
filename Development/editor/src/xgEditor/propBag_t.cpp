#include "stdafx.h"
#include <wx/event.h>
#include "eventHandler_t.h"
#include "prop_t.h"
#include "propBag_t.h"


namespace vega
{
	bool propBag_t::onEvent(wxEvent & event){
		bool run(true);
		if (event.GetEventObject() && wxIS_KIND_OF(event.GetEventObject(), wxWindow)){
			wxWindow *wd = wxStaticCast(event.GetEventObject(), wxWindow);
			while (run && wd){
				for (iterator itr(begin()); itr != end(); ++itr){
					if ((*itr).first == wd){
						(*itr).second->onEvent(event);
						run = false;
						break;
					}
				}
				if (run){
					wd = wd->GetParent();
				}
			}
		}
		return (bool)true;
	}
	propBag_t::~propBag_t(){
		for (iterator itr(begin()); itr != end(); ++itr){
			delete itr->second;
		}
		clear();
	}
}