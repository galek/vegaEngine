#pragma once

#include <string>
class wxTextCtrl;

namespace vega
{
	class log_t :
		public Ogre::LogListener
	{
	public:
		log_t(void);
		virtual ~log_t(void);
		void setLogWindow(wxTextCtrl *logWindow);
	private:
		void messageLogged(const Ogre::String &, Ogre::LogMessageLevel, bool, const Ogre::String &, bool &);
	private:
		wxTextCtrl *logWindow;
	};
}