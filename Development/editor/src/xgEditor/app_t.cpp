#include "stdafx.h"
#include "app_t.h"
#include "frame_t.h"
#include "cgSys_t.h"
#include "xml.h"


namespace vega
{
	/**
	*/
	IMPLEMENT_APP(app_t);
	/**
	*/
	app_t::app_t(void){
	}
	/**
	*/
	app_t::~app_t(void){
		FreeConsole();
	}
	/**
	*/
	int app_t::OnRun(){
		int ret;
		try{
			ret = wxApp::OnRun();
		}
		catch (Ogre::Exception& e) {
			std::cerr << "An exception has occured: " << e.getFullDescription();
			std::string desc = e.getFullDescription().c_str();
			desc += "\n do you want to save your current scene";
			int ret = MessageBoxA(NULL, desc.c_str(), "An exception has occured!", MB_YESNO | MB_ICONERROR | MB_TASKMODAL);
			if (ret == IDYES)
				frame->OnSaveAs(wxCommandEvent());
		}
		return ret;
	}
	/**
	*/
	bool app_t::OnInit(){
		frame = new frame_t(wxT("VegaEd"));
		// Show it
		frame->Show(true);

		return true;
	}
}

#ifdef USE_BOOST
#define BOOST_ALL_NO_LIB 1
#ifndef _DEBUG
#pragma comment(lib,"libboost_date_time-vc120-1_55.lib")
#pragma comment(lib,"libboost_thread-vc120-mt-1_55.lib")
#pragma comment(lib,"libboost_system-vc120-1_55.lib")
#else
#pragma comment(lib,"libboost_date_time-vc120-gd-1_55.lib")
#pragma comment(lib,"libboost_thread-vc120-mt-gd-1_55.lib")
#pragma comment(lib,"libboost_system-vc120-mt-gd-1_55.lib")
#endif
#endif