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

#include "moveObjectProp_t.h"
#include "LightPropertyEditor.h"
#include "entityProp_t.h"
#include "blankProp_t.h"

#include "cgSys_t.h"
#include "xgScene_t.h"
#include "selectListener_t.h"
#include "moveObjectPropBag_t.h"
#include "wx/choicebk.h"

#include "wx/listbase.h"

#include "cameraTool_t.h"
#include "gridTool_t.h"
#include "prsTool_t.h"
#include "sectorTool_t.h"
#include "resInsertTool_t.h"
#include "renderOptionTool_t.h"
#include "aiNaviTool_t.h"

namespace vega
{
	void frame_t::InitTools()
	{
		//cameraTool_t *ct=new cameraTool_t;
		toolBag->push_back(new cameraTool_t);
		toolBag->push_back(new gridTool_t);
		toolBag->push_back(new resInsertTool_t);

		prs = new prsTool_t();
		toolBag->push_back(prs);

		toolBag->push_back(new sectorTool_t);
		toolBag->push_back(new renderOptionTool_t);
		toolBag->push_back(new aiNaviTool_t);

		//toolBag->push_back(new LightPropertyEditor);
		//ct->init();
		for (toolBag_t::iterator itr(toolBag->begin()); itr != toolBag->end(); ++itr){
			(*itr)->init();
		}
		auto tempPtr = new LightPropertyEditor();

		moveObjectPropBag->addProp(tempPtr);
		updateBag->AddUpdatable(tempPtr);

		moveObjectPropBag->addProp(new entityProp_t);

		GetEditor()->GetEditorScene()->pushSelectListener(moveObjectPropBag);
	}
	void frame_t::resetTool(){
		for (toolBag_t::iterator itr(toolBag->begin()); itr != toolBag->end(); ++itr){
			(*itr)->onDeactive();
		}
	}
}