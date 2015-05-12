#pragma once

namespace vega
{
	class propBag_t :public std::map < wxWindow*, prop_t* > {
	public:
		~propBag_t();
		virtual bool onEvent(wxEvent & event);
	};
}