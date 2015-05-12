#include "StdAfx.h"
#include "log_t.h"
#include "resource.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#pragma warning(disable:4312)
#pragma warning(disable:4100)
using namespace std;

namespace vega
{
	/**
	*/
	log_t::log_t(void){
		Ogre::LogManager::getSingletonPtr()->getDefaultLog()->addListener(this);
	}
	/**
	*/
	log_t::~log_t(void){
	}
	/**
	*/
	void log_t::setLogWindow(wxTextCtrl *logWindow){
		this->logWindow = logWindow;
	}
	/**
	*/
	void log_t::messageLogged(const Ogre::String &_message, Ogre::LogMessageLevel, bool, const Ogre::String &, bool &){
		wxString ws;
		ws.Printf(wxT("%s\n"), _message);
		logWindow->AppendText(ws);
	}
}