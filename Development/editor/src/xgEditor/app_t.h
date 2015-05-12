#pragma once

#include "wx/wx.h"

namespace vega
{
	class app_t : public wxApp
	{
	public:
		app_t(void);
		~app_t(void);
		// Called on application startup
		virtual bool OnInit();
		virtual int OnRun();
	};
	// Implements MyApp& GetApp()
	DECLARE_APP(app_t)
}