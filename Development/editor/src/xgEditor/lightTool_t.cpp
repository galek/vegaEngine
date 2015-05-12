#include"stdafx.h"
#include "wx/generic/filedlgg.h"
#include "tool_t.h"
#include "wx/treebase.h"
#include"lightTool_t.h"
#include"frame_t.h"
#include <wx/treectrl.h>
#include "wx/choicebk.h"
#include "wx/splitter.h"
#include "wx/dirctrl.h"
#include "wx/treebook.h"
#include "cgSys_t.h"
#include "xgScene_t.h"
#include "mathUtil.h"
#include "msgSwitchHub_t.h"


namespace vega
{
	static const long idPointLight = wxNewId();
	static const long idDirectionalLight = wxNewId();
	static const long idSpotlightLight = wxNewId();
	BEGIN_EVENT_TABLE(lightTool_t, tool_t)
		END_EVENT_TABLE()
		void lightTool_t::onEvent(wxCommandEvent & event){
	}
	lightTool_t::lightTool_t(){
		manualObject = NULL;
		sceneNode = NULL;
		state = -1;
		id = 0;
	}
	bool lightTool_t::onActive(){
		active = true;
		return true;
	}
	bool lightTool_t::onDeactive(){
		active = false;
		return true;
	}
	bool lightTool_t::onViewMouseEvent(wxMouseEvent & event){
		switch (state){
		case 1:{
			long x, y;
			event.GetPosition(&x, &y);
			Ray ray;
			if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
				Plane plane(Vector3(0, 1, 0), 0);//sceneNode->getPosition().y
				std::pair< bool, Real > interPoint = Math::intersects(ray, plane);
				endPoint = ray.getPoint(interPoint.second);
				sceneNode->setPosition(update());
			}
			break; }
		case 2:{
			endPoint.y += oldMouseYPos - event.GetY();
			sceneNode->setPosition(update());
			break; }
		}
		if (event.ButtonDown(wxMOUSE_BTN_LEFT)){
			switch (state){
			case -1:{
				wxString name;
				name.Printf("%s_%d", "light", id++);
				manualObject = GetEditor()->GetEditorScene()->createManualObject(name.c_str());
				SceneManager *sm = GetEditor()->GetEditorScene();
				sceneNode = sm->createSceneNode(name.c_str());
				sm->getRootSceneNode()->addChild(sceneNode);
				sceneNode->attachObject(manualObject);
				long x, y;
				event.GetPosition(&x, &y);
				Ray ray;
				if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
					Plane plane(Vector3(0, 1, 0), 0);//sceneNode->getPosition().y
					std::pair< bool, Real > interPoint = Math::intersects(ray, plane);
					startPoint = ray.getPoint(interPoint.second);
					endPoint = startPoint + Vector3(1, 1, 1);
					sceneNode->setPosition(update());
					state = 1;
				}
				break; }
			case 1:{
				long x, y;
				event.GetPosition(&x, &y);
				Ray ray;
				if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
					Plane plane(Vector3(0, 1, 0), 0);//sceneNode->getPosition().y
					std::pair< bool, Real > interPoint = Math::intersects(ray, plane);
					endPoint = ray.getPoint(interPoint.second);
					sceneNode->setPosition(update());
					state = 2;
				}
				break; }
			case 2:{
				sceneNode->setPosition(update());
				state = -1;
				break; }

			}

		}
		//	event.GetPosition(&oldMouseXPos,&oldMouseYPos);
		oldMouseYPos = event.GetY();
		return true;
	}
	void lightTool_t::makeTrangle(const Vector3 *p, int a, int b, int c){
		ColourValue gray(0.0f, 0.6f, 0.0f, 0.5f);
		Vector3 normal = (p[c] - p[b]).crossProduct(p[b] - p[a]);
		normal.normalise();
		manualObject->position(p[a]);
		manualObject->normal(normal);
		//manualObject->textureCoord(0,0);
		manualObject->colour(gray);

		manualObject->position(p[b]);
		manualObject->normal(normal);
		//manualObject->textureCoord(0,0);
		manualObject->colour(gray);

		manualObject->position(p[c]);
		manualObject->normal(normal);
		//manualObject->textureCoord(0,0);
		manualObject->colour(gray);

	}
	Vector3 lightTool_t::update(){
		Vector3 center;
		if (manualObject){
			Vector3 minp;
			Vector3 maxp;
			AxisAlignedBox aab;
			aab.merge(startPoint);
			aab.merge(endPoint);
			center = aab.getCenter();
			Matrix4 mat(Matrix4::IDENTITY);
			mat.setTrans(-center);
			aab.transform(mat);
			manualObject->clear();
			manualObject->begin("gridMesh", RenderOperation::OT_TRIANGLE_LIST);
			const Vector3 *c = aab.getAllCorners();

			makeTrangle(c, 0, 3, 7);
			makeTrangle(c, 0, 7, 6);

			makeTrangle(c, 1, 5, 4);
			makeTrangle(c, 1, 4, 2);

			makeTrangle(c, 0, 6, 5);
			makeTrangle(c, 0, 5, 1);

			makeTrangle(c, 3, 2, 4);
			makeTrangle(c, 3, 4, 7);

			makeTrangle(c, 0, 1, 2);
			makeTrangle(c, 0, 2, 3);

			makeTrangle(c, 6, 7, 4);
			makeTrangle(c, 6, 4, 5);

			manualObject->end();
			sceneNode->_updateBounds();
		}
		return center;
	}
	bool lightTool_t::init(){
		wxWindow *wid = new wxWindow(frame->getToolBook(), wxID_ANY);

		wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
		box->Add(new wxButton(wid, idPointLight, "point light"));
		box->Add(new wxButton(wid, idDirectionalLight, "directional light"));
		box->Add(new wxButton(wid, idSpotlightLight, "spotlight light"));

		box->Fit(wid);
		box->Layout();
		wid->SetSizer(box);

		frame->getToolBook()->AddPage(wid, wxT("light"));
		frame->getMsgSwitchHub()->link(wid, this);
		return true;
	}
	lightTool_t::~lightTool_t(){
	}
}