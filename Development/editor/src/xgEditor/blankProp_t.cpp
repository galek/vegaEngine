#include "stdafx.h"
#include "moveObjectProp_t.h"
#include "frame_t.h"
#include "blankProp_t.h"
#include "wx/choicebk.h"
#include "updateBag_t.h"
#include "wx/listctrl.h"

namespace vega
{
	static const long idP = wxNewId();
	static const long idR = wxNewId();
	static const long idRY = wxNewId();
	static const long idS = wxNewId();
	static const long idList = wxNewId();
	bool blankProp_t::init(){
		wxWindow *wid = this;//new wxWindow(frame->getPropBook(),wxID_ANY);
		Create(frame->getPropBook(), wxID_ANY);
		frame->getPropBook()->AddPage(wid, wxT("blankProp"));
		index = frame->getPropBook()->GetPageCount() - 1;
		typeName = "";
		return true;
	}
	blankProp_t::blankProp_t(){

	}
	void blankProp_t::update(Ogre::MovableObject *o, bool to){

	}
}