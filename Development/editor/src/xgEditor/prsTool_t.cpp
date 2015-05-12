#include"stdafx.h"
#include "wx/generic/filedlgg.h"
#include "tool_t.h"
#include "wx/treebase.h"
#include"prsTool_t.h"
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
#include <wx/vscroll.h> 
#include <wx/vlbox.h> 
#include <wx/scrolwin.h> 


#include "cgSys_t.h"
#include "ToolsIds.h"

namespace vega
{
	void prsTool_t::onEvent(wxCommandEvent & event){
		buttonId = event.GetId();
	}
	prsTool_t::prsTool_t(){
		sceneNode = NULL;
		buttonId = IDM_TOOLBAR_TOOL_MOVE;
	}
	bool prsTool_t::onActive(){
		active = true;
		frame->setViewMsgRecvTool(this);
		return true;
	}
	bool prsTool_t::onDeactive(){
		frame->setViewMsgRecvTool(NULL);
		active = false;
		sceneNode = NULL;
		return true;
	}
	bool prsTool_t::Show(bool show){
		if (show){
			onActive();
		}
		else{
			onDeactive();
		}
		return wxWindow::Show(show);
	}
	void prsTool_t::updatePos(wxMouseEvent & event){
		//SeriousWarning(true, "prsTool_t::updatePos");
		if (sceneNode){
			if (event.ControlDown()){
				if (event.LeftIsDown()){
					long x, y;
					event.GetPosition(&x, &y);
					Ogre::Ray ray;
					if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
						Ogre::Vector3 normal = -ray.getDirection();
						normal.y = 0;
						Ogre::Plane plane(normal, oldPostion/*sceneNode->getPosition()*/);
						std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
						if (interPoint.first)
						{
							Ogre::Vector3 offset = ray.getPoint(interPoint.second) - oldPoint;
							sceneNode->setPosition(sceneNode->getPosition() + offset);
							oldPoint = ray.getPoint(interPoint.second);
						}
					}
					/*
									Vector3 p=sceneNode->getPosition();
									p.y+=(oldMouseYPos-event.GetY() )*0.1f;
									sceneNode->setPosition(p);*/
				}
			}
			else{
				if (event.LeftIsDown()){
					long x, y;
					event.GetPosition(&x, &y);
					Ogre::Ray ray;
					if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
						Ogre::Plane plane(Ogre::Vector3(0, 1, 0), sceneNode->getPosition().y);
						std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
						if (interPoint.first){
							sceneNode->setPosition(sceneNode->getPosition() + ray.getPoint(interPoint.second) - oldPoint);
							oldPoint = ray.getPoint(interPoint.second);
							oldPostion = oldPoint;
						}
					}
				}
			}
		}
	}
	void prsTool_t::updateRot(wxMouseEvent & event){
		if (sceneNode){
			if (event.LeftIsDown()){
				long x, y;
				event.GetPosition(&x, &y);
				Ogre::Ray ray;
				if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
					Ogre::Radian rfAngle;
					Ogre::Vector3 axis;
					if (event.ControlDown()){
						axis = ray.getDirection();
						rfAngle = (oldMouseXPos - x) + (oldMouseYPos - y);
					}
					else{
						axis = oldRay.getDirection().crossProduct(ray.getDirection());
						axis.normalise();
						rfAngle = sqrtf((x - oldMouseXPos)*(x - oldMouseXPos) + (y - oldMouseYPos)*(y - oldMouseYPos));
					}
					Ogre::Quaternion q(-rfAngle*0.01f, axis);
					sceneNode->rotate(q, Ogre::Node::TS_WORLD);
					sceneNode->_updateBounds();
					oldRay = ray;
				}
			}
		}
	}
	void prsTool_t::updateRotY(wxMouseEvent & event){
		if (sceneNode){
			if (event.LeftIsDown()){
				long x, y;
				event.GetPosition(&x, &y);
				Ogre::Radian rfAngle;
				Ogre::Vector3 axis(0, 1, 0);
				rfAngle = ((x - oldMouseXPos) + (y - oldMouseYPos));
				if (event.ControlDown()){
					axis.x = 1;
					axis.y = 0;
					axis.z = 0;
				}
				else if (event.AltDown()){
					axis.x = 0;
					axis.y = 0;
					axis.z = 1;
				}
				Ogre::Quaternion q(-rfAngle*0.01f, axis);
				sceneNode->rotate(q, Ogre::Node::TS_WORLD);
				sceneNode->_updateBounds();
			}
		}
	}
	void prsTool_t::updateScale(wxMouseEvent & event){
		if (sceneNode){
			if (event.LeftIsDown()){
				long x, y;
				event.GetPosition(&x, &y);
				Ogre::Ray ray;
				if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
					Ogre::Radian rfAngle;
					Ogre::Vector3 axis(0, 0, 0);
					if (event.ControlDown()){
						axis.y = (oldMouseYPos - y);
						axis *= 0.01f;
					}
					else{
						Ogre::Plane plane(Ogre::Vector3(0, 1, 0), sceneNode->getPosition().y);
						std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
						axis = ray.getPoint(interPoint.second) - oldPoint;
						axis *= 0.01f;
						oldPoint = ray.getPoint(interPoint.second);
						axis = sceneNode->getOrientation().Inverse()*axis;
					}
					sceneNode->setScale(sceneNode->getScale() + axis);
					sceneNode->_updateBounds();
				}
			}
		}
	}

	bool prsTool_t::onViewKeyEvent(wxKeyEvent & event){
		if (event.GetKeyCode() == WXK_DELETE){
			if (sceneNode){
				while (sceneNode->numAttachedObjects()){
					Ogre::MovableObject *m = sceneNode->getAttachedObject(0);
					GetEditor()->GetEditorScene()->destroyMovableObject(m->getName(), m->_getCreator()->getType());
				}
				GetEditor()->GetEditorScene()->setSelSceneNode(NULL);
				GetEditor()->GetEditorScene()->destroySceneNode(sceneNode->getName());
				sceneNode = NULL;
			}
			return true;
		}
		return false;
	}
	bool prsTool_t::onViewMouseEvent(wxMouseEvent & event)
	{
		//SeriousWarning(true, " prsTool_t::onViewMouseEvent");
		if (event.ButtonDown(wxMOUSE_BTN_LEFT))
		{
			//	SeriousWarning(true, "event.ButtonDown(wxMOUSE_BTN_LEFT)");
			long x, y;
			event.GetPosition(&x, &y);
			Ogre::Ray ray;
			if (mathUtil_t::getViewRay(x, y, ray, GetEditor()->GetEditorScene()->getCurrentViewport())){
				oldRay = ray;
				Ogre::RaySceneQuery *rsq = GetEditor()->GetEditorScene()->createRayQuery(ray);
				rsq->setSortByDistance(true, 10);
				rsq->setQueryTypeMask(rsq->getQueryTypeMask() | Ogre::SceneManager::LIGHT_TYPE_MASK | Ogre::SceneManager::FX_TYPE_MASK);
				Ogre::RaySceneQueryResult &rsqr = rsq->execute();
				int count = rsqr.size();
				if (rsqr.size()){
					Ogre::RaySceneQueryResult::iterator itr(rsqr.begin());
					Ogre::RaySceneQueryResult::iterator itr2;
					for (; itr != rsqr.end(); ++itr){
						itr2 = itr;
						++itr2;
						if (itr2 != rsqr.end()){
							if ((*itr).movable->getParentSceneNode()->_getWorldAABB().contains((*itr2).movable->getParentSceneNode()->_getWorldAABB()))
								continue;
						}

						if ((*itr).movable && !(*itr).movable->getParentSceneNode()->_getWorldAABB().intersects(ray.getOrigin())){
							type_t *t = (type_t*)&((*itr).movable->getUserAny());
							if (t->editable || t->dummy){
								sceneNode = (*itr).movable->getParentSceneNode();
								Ogre::MovableObject *moveAbleObject = (*itr).movable;
								if (t->dummy){
									moveAbleObject = NULL;
									int numAttachedObjects = sceneNode->numAttachedObjects();
									for (int i(0); i < numAttachedObjects; ++i){
										Ogre::MovableObject *m = sceneNode->getAttachedObject(i);
										type_t* tp = (type_t*)&(m->getUserAny());
										if (tp->editable && !tp->dummy){
											moveAbleObject = m;
											break;
										}
									}
								}

								Ogre::Plane plane(Ogre::Vector3(0, 1, 0), sceneNode->getPosition().y);
								std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
								oldPoint = ray.getPoint(interPoint.second);
								oldPostion = oldPoint;
								GetEditor()->GetEditorScene()->setSelSceneNode(sceneNode);
								GetEditor()->GetEditorScene()->setSelMovable(moveAbleObject);
								break;
							}
							else{
								GetEditor()->GetEditorScene()->setSelSceneNode(NULL);
								GetEditor()->GetEditorScene()->setSelMovable(NULL);
								sceneNode = NULL;
							}

						}
					}
					if (itr == rsqr.end()){
						GetEditor()->GetEditorScene()->setSelSceneNode(NULL);
						GetEditor()->GetEditorScene()->setSelMovable(NULL);
					}

				}/*
				else{
				GetEditor()->GetEditorScene()->setSelSceneNode(NULL);
				sceneNode=NULL;
				}*/

				GetEditor()->GetEditorScene()->destroyQuery(rsq);
			}
		}
		else
		{
			//if (buttonId == IDM_TOOLBAR_TOOL_MOVE)
			{
				updatePos(event);
			}
			/*else if (buttonId == IDM_TOOLBAR_TOOL_ROTATE)
			{
				updateRot(event);
			}
			else if (buttonId == IDM_TOOLBAR_TOOL_SCALE){
				updateScale(event);
			}
			else if (buttonId == IDM_TOOLBAR_TOOL_ROTATE_Y){
				updateRotY(event);
			}*/
		}
		event.GetPosition(&oldMouseXPos, &oldMouseYPos);
		return true;
	}

	bool prsTool_t::init()
	{
#if 0
		wxWindow *wid = this;//new wxWindow(frame->getToolBook(),wxID_ANY);
		int i(0);
		int width(80);
		int height(50);
		box = new wxBoxSizer(wxVERTICAL);
		move = new wxButton(wid, IDM_TOOLBAR_TOOL_MOVE, "pos_free");
		box->Add(move);
		box->Add(new wxButton(wid, IDM_TOOLBAR_TOOL_ROTATE, "rot_free"));
		wxButton *ryb = new wxButton(wid, IDM_TOOLBAR_TOOL_ROTATE_Y, "rot_axis"/*,wxPoint(0,i),wxSize(width,height)*/);
		wxButton *sb = new wxButton(wid, IDM_TOOLBAR_TOOL_SCALE, "scale"/*,wxPoint(width,i),wxSize(width,height)*/);
		box->Add(ryb);
		box->Add(sb);
		box->Fit(wid);
		box->Layout();
		wid->SetSizer(box);
		frame->getToolBook()->AddPage(wid, wxT("prs"));
		//frame->getMsgSwitchHub()->link(wid,this);
#endif
		return true;
	}
	prsTool_t::~prsTool_t(){
	}
}