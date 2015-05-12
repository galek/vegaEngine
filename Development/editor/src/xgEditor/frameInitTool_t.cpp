#include "stdafx.h"	
#include <wx/treectrl.h>
#include "wx/noteBook.h"
#include "frame_t.h"
#include "docInfoList_t.h"
#include "wx/file.h"
#include "alarm_t.h"
#include "wx/generic/choicdgg.h"
#include <wx/dynlib.h> 
#include "wxNotebookEx.h"
#include "toolBag_t.h"

#include "wxOgre_t.h"
#include "updateBag_t.h"
//_________________________________________
#include "moveObjectProp_t.h"
#include "lightProp_t.h"
#include "entityProp_t.h"
#include "blankProp_t.h"
//_________________________________________
#include "cgSys_t.h"
#include "xgScene_t.h"
#include "selectListener_t.h"
#include "moveObjectPropBag_t.h"
#include "wx/choicebk.h"

#include "wx/listbase.h"
//_________________________________________
#include "cameraTool_t.h"
#include "gridTool_t.h"
//#include "testTool_t.h"
#include "prsTool_t.h"
#include "sectorTool_t.h"
#include "resInsertTool_t.h"
#include "renderOptionTool_t.h"
#include "aiNaviTool_t.h"

namespace vega
{
	void frame_t::initTool(){
		//toolBag->push_back(new testTool_t);
		//cameraTool_t *ct=new cameraTool_t;
		toolBag->push_back(new cameraTool_t);
		toolBag->push_back(new gridTool_t);
		toolBag->push_back(new resInsertTool_t);

		prs = new prsTool_t();
		toolBag->push_back(prs);

		toolBag->push_back(new sectorTool_t);
		toolBag->push_back(new renderOptionTool_t);
		toolBag->push_back(new aiNaviTool_t);

		//toolBag->push_back(new lightProp_t);
		//ct->init();
		for (toolBag_t::iterator itr(toolBag->begin()); itr != toolBag->end(); ++itr){
			(*itr)->init();
		}
		moveObjectPropBag->addProp(new lightProp_t);
		moveObjectPropBag->addProp(new entityProp_t);
		moveObjectPropBag->addProp(new blankProp_t);

		GetEditor()->GetEditorScene()->pushSelectListener(moveObjectPropBag);
		wxWindow *wid = new wxWindow(frame->getPropBook(), wxID_ANY);
		frame->getPropBook()->AddPage(wid, wxT("blankProp"), true);
	}
	void frame_t::resetTool(){
		for (toolBag_t::iterator itr(toolBag->begin()); itr != toolBag->end(); ++itr){
			(*itr)->onDeactive();
		}
	}
}