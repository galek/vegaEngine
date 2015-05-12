/*
Copyright (c) 2005-2007, lidong wang

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of the lidongwang nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "stdafx.h"
#include "moveObjectProp_t.h"
#include "frame_t.h"
#include "entityProp_t.h"
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
	static const long animMode = wxNewId();
	BEGIN_EVENT_TABLE(entityProp_t, wxWindow)
		EVT_LIST_ITEM_SELECTED(idList, entityProp_t::onColClick)
		//EVT_LIST_ITEM_RIGHT_CLICK(idList, entityProp_t::onRightClick)
		END_EVENT_TABLE()
	class myListCtrl_t :public wxListCtrl{
		DECLARE_EVENT_TABLE()
		entityProp_t *p;
	public:
		myListCtrl_t(entityProp_t *p, wxWindow *parent,
			const wxWindowID id,
			const wxPoint& pos,
			const wxSize& size,
			long style)
			: wxListCtrl(parent, id, pos, size, style), p(p)
		{
#ifdef __POCKETPC__
			EnableContextMenu();
#endif
		}
		void onColClick(wxListEvent& event){
			p->onColClick(event);
		}
	};
	BEGIN_EVENT_TABLE(myListCtrl_t, wxListCtrl)
		EVT_LIST_ITEM_SELECTED(idList, myListCtrl_t::onColClick)
		END_EVENT_TABLE()
		bool entityProp_t::init(){
		e = NULL;

		frame->getUpdateBag()->push_back(this);
		wxPanel *wid = this;//new wxWindow(frame->getPropBook(),wxID_ANY);
		Create(frame->getPropBook(), wxID_ANY);
		//wlc=new myListCtrl_t(this,wid,idList,wxDefaultPosition,wxSize(250,300),wxLC_ICON);
		wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
		wlc = new wxListCtrl(wid, idList, wxDefaultPosition, wxSize(250, 100), wxLC_ICON);
		box->Add(wlc, 1, wxEXPAND | wxALL, 2);
		box->Add(
			new wxStaticText(wid, wxID_ANY, "you can select with ctrl key to select more anim to play them at the same time", wxPoint(0, 101), wxSize(250, 100)), 1, wxEXPAND | wxALL, 2);
		wid->SetSizer(box);
		box->Fit(wid);
		box->SetSizeHints(wid);
		/*int i(0);
		int width(80);
		int height(50);
		wxBoxSizer *box=new wxBoxSizer(wxHORIZONTAL);
		box->Add(new wxButton(wid,idP,"pos"));
		box->Add(new wxButton(wid,idR,"rot_free"));
		wid->SetSizer(box);
		box->Fit (wid);
		box->Layout();
		i+=height;
		wxButton *ryb=new wxButton(wid,idRY,"rot_axis",wxPoint(0,i),wxSize(width,height));
		wxButton *sb=new wxButton(wid,idS,"scale",wxPoint(width,i),wxSize(width,height));
		wid->Layout();*/
		frame->getPropBook()->AddPage(wid, wxT("entityProp"));
		index = frame->getPropBook()->GetPageCount() - 1;
		typeName = Ogre::EntityFactory::FACTORY_TYPE_NAME;
		return true;
	}
	entityProp_t::entityProp_t(){
	}
	void entityProp_t::onRightClick(wxListEvent& event){
		try{
			if (e && e->hasSkeleton()){
				const wxString &name = event.GetLabel();
				e->getAnimationState(name.c_str().AsChar())->setEnabled(true);
				e->getAnimationState(name.c_str().AsChar())->setLoop(true);
			}
		}
		catch (Ogre::Exception& e) {
#ifdef WIN32
			MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
	}
}
	void entityProp_t::onColClick(wxListEvent& event){
		if (e && e->hasSkeleton()){
			Ogre::AnimationStateSet *ass = e->getAllAnimationStates();
			if (ass){
				Ogre::ConstEnabledAnimationStateIterator ceasi = ass->getEnabledAnimationStateIterator();

				while (ceasi.hasMoreElements()){
					auto next = ceasi.peekNext();
					next->setEnabled(false);

					ceasi = ass->getEnabledAnimationStateIterator();
				}
			}
			int size = wlc->GetSelectedItemCount();
			int itemCount = wlc->GetItemCount();
			for (int i = 0; i < itemCount&&size; ++i){
				if (wlc->GetItemState(i, wxLIST_STATE_SELECTED)){
					const wxString &name = wlc->GetItemText(i);
					e->getAnimationState(name.c_str().AsChar())->setEnabled(true);
					e->getAnimationState(name.c_str().AsChar())->setLoop(true);
					--size;
				}
			}
			//const wxString &name=event.GetLabel();
		}
	}
	bool entityProp_t::Show(bool show){
		if (show){
			e = NULL;
			onActive();

		}
		else{
			//frame->getUpdateBag()->remove(this);
			e = NULL;
			onDeactive();
		}
		return wxWindow::Show(show);
	}
	entityProp_t::~entityProp_t(){
	}
	void entityProp_t::update(float t){
		if (e && e->hasSkeleton()){
			Ogre::AnimationStateSet *ass = e->getAllAnimationStates();
			if (ass){
				Ogre::ConstEnabledAnimationStateIterator ceasi = (ass->getEnabledAnimationStateIterator());
				while (ceasi.hasMoreElements()){
					auto next = ceasi.peekNext();
					next->addTime(t);
					ceasi.moveNext();
				}
			}
		}
	}
	void entityProp_t::update(Ogre::MovableObject *o, bool to){
		wlc->ClearAll();
		e = static_cast<Ogre::Entity *>(o);
		if (e && e->hasSkeleton()){
			Ogre::AnimationStateSet *ass = e->getAllAnimationStates();
			if (ass){
				Ogre::AnimationStateIterator asi = ass->getAnimationStateIterator();
				while (asi.hasMoreElements()){
					Ogre::String animName = asi.peekNextKey();
					asi.moveNext();
					wlc->InsertItem(0, animName);
				}
			}
		}
		/*
			if( o->hasSkeleton() &&  o->getAnimationState("swim")){
			o->getAnimationState("swim")->setEnabled(true);
			o->getAnimationState("swim")->setLoop(true);
			static float inc=0;
			o->getAnimationState("swim")->addTime(inc);
			inc+=0.2;
			}*/
	}
}