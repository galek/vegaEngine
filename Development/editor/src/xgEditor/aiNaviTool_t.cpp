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
#include"stdafx.h"
#include "wx/generic/filedlgg.h"
#include "tool_t.h"
#include "wx/treebase.h"
#include"aiNaviTool_t.h"
#include"frame_t.h"
#include <wx/treectrl.h>
#include "wx/choicebk.h"
#include "wx/splitter.h"
#include "wx/dirctrl.h"
#include "wx/treebook.h"
#include "cgSys_t.h"
#include "xgScene_t.h"
#include "mathUtil.h"
#include "type_t.h"
#include "dummyColors.h"
#include "xgstring.h"

namespace vega
{
	BEGIN_EVENT_TABLE(aiNaviTool_t, wxWindow)
		END_EVENT_TABLE()
		void aiNaviTool_t::onEvent(wxCommandEvent & event){
	}
	aiNaviTool_t::aiNaviTool_t(){
		manualObject = NULL;
		sceneNode = NULL;
		state = -1;
		id = 0;
	}
	bool aiNaviTool_t::onActive(){
		frame->setViewMsgRecvTool(this);
		active = true;
		return true;
	}
	bool aiNaviTool_t::onDeactive(){
		frame->setViewMsgRecvTool(NULL);
		active = false;
		return true;
	}
	bool aiNaviTool_t::Show(bool show){
		if (show){
			onActive();
		}
		else{
			onDeactive();
		}
		return wxWindow::Show(show);
	}
	bool aiNaviTool_t::onViewMouseEvent(wxMouseEvent & event)
	{
		switch (state)
		{
		case 1:
		{
			long x, y;
			event.GetPosition(&x, &y);
			Ogre::Ray ray;
			if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
				Ogre::Plane plane(Ogre::Vector3(0, 1, 0), 0);//sceneNode->getPosition().y
				std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
				endPoint = ray.getPoint(interPoint.second);
				sceneNode->setPosition(update());
			}
			break;
		}

		case 2:
		{
			endPoint.y += oldMouseYPos - event.GetY();
			sceneNode->setPosition(update());
			break;
		}
		}

		if (event.ButtonDown(wxMOUSE_BTN_LEFT))
		{
			switch (state)
			{
			case -1:
			{
				xgstring name;
				name.printf("%s_%d", "node", id++);
				while (GetEditor()->GetEditorScene()->hasManualObject(name.c_str()))
					name.printf("%s_%d", "node", id++);
				manualObject = GetEditor()->GetEditorScene()->createManualObject(name.c_str());
				manualObject->setUserAny(*new type_t(ai_navi_type_));
				Ogre::SceneManager *sm = GetEditor()->mGSceneMgr;
				sceneNode = sm->createSceneNode(name.c_str());
				sm->getRootSceneNode()->addChild(sceneNode);
				sceneNode->attachObject(manualObject);
				long x, y;
				event.GetPosition(&x, &y);
				Ogre::Ray ray;
				if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
					Ogre::Plane plane(Ogre::Vector3(0, 1, 0), 0);//sceneNode->getPosition().y
					std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
					startPoint = ray.getPoint(interPoint.second);
					//endPoint=startPoint+Vector3(0.01,0.01,0.01);
					endPoint = startPoint;
					sceneNode->setPosition(update());
					state = 1;
				}
				break;
			}

			case 1:
			{
				long x, y;
				event.GetPosition(&x, &y);
				Ogre::Ray ray;
				if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport()))
				{
					Ogre::Plane plane(Ogre::Vector3(0, 1, 0), 0);//sceneNode->getPosition().y
					std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
					endPoint = ray.getPoint(interPoint.second);
					sceneNode->setPosition(update());
					state = 2;
				}
				break;
			}

			case 2:
			{
				sceneNode->setPosition(update());
				state = -1;
				break;
			}

			}

		}
		//	event.GetPosition(&oldMouseXPos,&oldMouseYPos);
		oldMouseYPos = event.GetY();
		return true;
	}

	void aiNaviTool_t::makeTrangle(const Ogre::Vector3 *p, int a, int b, int c){
		Ogre::ColourValue gray(AI_NAVI_COLOR);
		Ogre::Vector3 normal = (p[c] - p[b]).crossProduct(p[b] - p[a]);
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

	Ogre::Vector3 aiNaviTool_t::update(){
		Ogre::Vector3 center;
		if (manualObject)
		{
			//Vector3 minp;
			//Vector3 maxp;
			Ogre::AxisAlignedBox aab;
			aab.setNull();
			aab.merge(startPoint);
			aab.merge(endPoint);
			center = aab.getCenter();
			Ogre::Matrix4 mat(Ogre::Matrix4::IDENTITY);
			mat.setTrans(-center);
			aab.transformAffine(mat);
			manualObject->clear();
			manualObject->begin("aiNaviMesh", Ogre::RenderOperation::OT_TRIANGLE_LIST);
			const Ogre::Vector3 *c = aab.getAllCorners();

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

	bool aiNaviTool_t::init(){
		wxWindow *wid = this;//new wxWindow(frame->getToolBook(),wxID_ANY);
		Create(frame->getToolBook(), wxID_ANY);
		frame->getToolBook()->AddPage(wid, wxT("ai navi"));
		return true;
	}

	aiNaviTool_t::~aiNaviTool_t(){
	}
}