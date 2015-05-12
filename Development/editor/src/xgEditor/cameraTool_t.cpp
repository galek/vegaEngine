#include "stdafx.h"
#include <wx/listbook.h>
#include <wx/treebook.h>
#include "cameraTool_t.h"
#include "frame_t.h"
#include "wxOgre_t.h"
#include "updateBag_t.h"
using namespace Ogre;


namespace vega
{
	bool cameraTool_t::onViewMouseEvent(wxMouseEvent& event)
	{
		Camera *c = frame->getView()->getCamera();
		if (event.ButtonDown(wxMOUSE_BTN_RIGHT)){
			frame->SetFocus();
		}
		if (c){
			if (event.Dragging() && event.RightIsDown()){
				wxPoint rp = event.GetPosition() - oldPoint;
				float yaw = rp.x*rotVel;
				float pitch = rp.y*rotVel;
				Radian x(yaw);
				Radian y(pitch);
				c->yaw(x);
				c->pitch(y);
				//Quaternion q=c->getOrientation();
				//Vector3 v=c->getPosition();
			}
			oldPoint = event.GetPosition();
		}
		// TODO: Add your specialized code here.
		return (bool)0;
	}


	void cameraTool_t::updateCamRotation(){
		Camera *c = frame->getView()->getCamera();
		Radian x(xo);
		Radian y(yo);
		c->yaw(x);
		c->pitch(y);
	}

	void cameraTool_t::update(float t)
	{
		if (!frame->IsActive())
			return;
		/*if(!frame->getView()->IsFocused ())
			return;*/
		Camera *c = frame->getView()->getCamera();
		float dt = t;
		float moveVelT = moveVel*(500.0f);
		Vector3 currentCameraPostion;
		if (GetAsyncKeyState('W') & 0x8000){
			Vector3  t = moveVelT*c->getRealDirection()*dt;
			c->setPosition(c->getRealPosition() + t);
			currentCameraPostion += t;
			//c->Update(0);
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState('S') & 0x8000){
			Vector3  t = -moveVelT*c->getRealDirection()*dt;
			c->setPosition(c->getRealPosition() + t);
			currentCameraPostion += t;
			//c->Update(0);
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState('A') & 0x8000){
			Vector3  t = -moveVelT*c->getRealRight()*dt;
			c->setPosition(c->getRealPosition() + t);
			currentCameraPostion += t;
			//c->Update(0);
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState('D') & 0x8000){
			Vector3  t = moveVelT*c->getRealRight()*dt;
			c->setPosition(c->getRealPosition() + t);
			currentCameraPostion += t;
			//c->Update(0);
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState(' ') & 0x8000){
			Vector3  t = moveVelT*c->getRealUp()*dt;
			c->setPosition(c->getRealPosition() + t);
			currentCameraPostion += t;
			//c->Update(0);
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000){
			Vector3  t = -moveVelT*c->getRealUp()*dt;
			c->setPosition(c->getRealPosition() + t);
			currentCameraPostion += t;
			//c->Update(0);
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
	{
		xo = 0;
		yo = 0;
		if (GetAsyncKeyState('Q') & 0x8000){
			//c->Orbit(currentCameraPostion,200.0f*mouseSensitiveScroll*dt,0);
			xo += rotVel;
			updateCamRotation();
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState('E') & 0x8000){
			//c->Orbit(currentCameraPostion,-200.0f*mouseSensitiveScroll*dt,0);
			xo -= rotVel;
			updateCamRotation();
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState('R') & 0x8000){
			//c->Orbit(currentCameraPostion,0,-200.0f*mouseSensitiveScroll*dt);
			yo += rotVel;
			updateCamRotation();
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		if (GetAsyncKeyState('F') & 0x8000){
			//c->Orbit(currentCameraPostion,0,200.0f*mouseSensitiveScroll*dt);
			yo -= rotVel;
			updateCamRotation();
			//plc->SetReferenceFrame(c->GetPosition(),c->GetOrientation());
		}
		float delta = t * 10;
		if (GetAsyncKeyState(VK_UP) & 0x8000){
			moveVel += delta;
			moveVel = moveVel < 0 ? 0 : moveVel;
			//moveVelProp->SetValue((_variant_t) ((long) (moveVel*100)));
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000){
			moveVel -= delta;
			moveVel = moveVel < 0 ? 0 : moveVel;
			//moveVelProp->SetValue((_variant_t) ((long) (moveVel*100)));
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000){
			rotVel -= delta;
			rotVel = rotVel < 0 ? 0 : rotVel;
			//rotVelProp->SetValue((_variant_t) ((long) (rotVel*10000)));
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000){
			rotVel += delta;
			rotVel = rotVel < 0 ? 0 : rotVel;
			//rotVelProp->SetValue((_variant_t) ((long) (rotVel*10000)));
		}
	}
	}
	static const long idMoveSlider = wxNewId();
	static const long idRotSlider = wxNewId();

	BEGIN_EVENT_TABLE(cameraTool_t, wxWindow)
		EVT_SLIDER(idMoveSlider, OnSliderEvent)
		EVT_SLIDER(idRotSlider, OnSliderEvent)


		END_EVENT_TABLE()

		void cameraTool_t::OnSliderEvent(wxCommandEvent& event){
		int id = event.GetId();
		if (idMoveSlider == id){
			//Beep(2000,200);
			moveVel = event.GetInt()*(0.5f / 100);
		}
		else if (id == idRotSlider){
			rotVel = event.GetInt()*(0.01f / 100);
		}
	}
	bool cameraTool_t::init()
	{
		rotVel = 0.01f;
		wxPanel *wid = this;
		if (!Create(frame->getToolBook(), wxID_ANY)){
			return false;
		}
		wid = new wxPanel(this);

		wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);

		buttonSizer->Add(
			new wxStaticText(wid, wxID_ANY,
			wxT("xy vel"),
			wxDefaultPosition, wxSize(300, 20), wxALIGN_CENTER));


		// Create text ctrl with minimal size 100x60
		buttonSizer->Add(
			new wxSlider(wid, idMoveSlider, 100, 1, 200,
			wxDefaultPosition, wxSize(300, 50),
			wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS | wxSL_SELRANGE, wxDefaultValidator, "xy vel"), 1, wxEXPAND | wxALL, 1);


		buttonSizer->Add(
			new wxStaticText(wid, wxID_ANY,
			wxT("rot vel"),
			wxPoint(80, 0), wxSize(300, 20), wxALIGN_CENTER));
		//buttonSizer->Add(new wxButton(wid,-1,"point light"),1,wxEXPAND|wxALL,2);

		// Create text ctrl with minimal size 100x60
		buttonSizer->Add(
			new wxSlider(wid, idRotSlider, 100, 1, 200,
			wxDefaultPosition, wxSize(300, 50),
			wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS | wxSL_SELRANGE, wxDefaultValidator, "rot vel"), 1, wxEXPAND | wxALL, 1);
		/*
			buttonSizer->Add(
			new wxButton( this, wxID_CANCEL, "Cancel" ),
			0,         // make horizontally unstretchable
			wxALL,     // make border all around (implicit top alignment)
			10 );      // set border width to 10
			*/
		/*
			topSizer->Add(
			buttonSizer,
			0,                // make vertically unstretchable
			wxALIGN_CENTER ); // no border and centre horizontally*/


		wid->SetSizer(buttonSizer); // use the sizer for layout
		buttonSizer->Fit(wid);          // fit the dialog to the contents
		buttonSizer->SetSizeHints(wid); // set hints to honor min size
		//buttonSizer->Layout();

		// TODO: Add your specialized code here.

		frame->getToolBook()->AddPage(this, wxT("camera"));
		frame->pushViewMsgRecvTool(this);
		frame->getUpdateBag()->push_back(this);
		moveVel = 0.4f;
		Camera *c = frame->getView()->getCamera();
		if (c){
			c->setOrientation(Quaternion(0.99998748, -0.0049999789, 0.00000000, 0));
			c->setPosition(0.00000000, 16.000000, 112.00000);
		}
		return true;
	}
}