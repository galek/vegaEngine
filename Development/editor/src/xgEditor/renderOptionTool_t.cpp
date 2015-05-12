#include "stdafx.h"
#include <wx/listbook.h>
#include <wx/treebook.h>
#include "renderOptionTool_t.h"
#include <wx/wxchar.h>

#include "wx/checkbox.h"
#include "cgSys_t.h"
#include "xgScene_t.h"
#include "frame_t.h"
#include "wxOgre_t.h"
#include "frame_t.h"

namespace vega
{
	static const long id_render_in_game_mode = wxNewId();
	static const long id_render_sector = wxNewId();
	static const long id_render_light = wxNewId();
	static const long id_render_sound = wxNewId();
	static const long id_render_mesh = wxNewId();
	static const long id_render_solid = wxNewId();
	static const long id_render_wireframe = wxNewId();
	static const long id_render_point = wxNewId();
	static const long id_reload_all_resouce = wxNewId();
	static const long id_unloadUnreferencedResources = wxNewId();

	BEGIN_EVENT_TABLE(renderOptionTool_t, wxWindow)
		EVT_CHECKBOX(id_render_in_game_mode, onButtonEvent)
		EVT_CHECKBOX(id_render_sector, onButtonEvent)
		EVT_CHECKBOX(id_render_light, onButtonEvent)
		EVT_CHECKBOX(id_render_sound, onButtonEvent)
		EVT_BUTTON(id_render_solid, onButtonEvent)
		EVT_BUTTON(id_render_wireframe, onButtonEvent)
		EVT_BUTTON(id_render_point, onButtonEvent)
		EVT_BUTTON(id_reload_all_resouce, onButtonEvent)
		EVT_BUTTON(id_unloadUnreferencedResources, onButtonEvent)
		END_EVENT_TABLE()
#include"frame_t.h"
		//##ModelId=44B9E6E7033C
		renderOptionTool_t::renderOptionTool_t(){
		wxWindow *wid = this;//=new wxWindow(frame->getToolBook(),wxID_ANY);
		Create(frame->getToolBook(), wxID_ANY);
		/*
		wxButton *wb=new wxButton(wid,test_button_id,L"renderOptionTool_t");
		wxTextCtrl *tc=new wxTextCtrl(wid,1,wxT("hellow"),wxPoint(0,100));*/

		wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

		wxCheckBox *checkbox;
		checkbox = new wxCheckBox(wid, id_render_in_game_mode,
			wxT("&render in game mode"));
		checkbox->SetValue(false);
		box->Add(checkbox);

		/*
			checkbox = new wxCheckBox(wid, id_render_sector,
			wxT("&show sector"));
			checkbox->SetValue(true);
			box->Add(checkbox);*/


		checkbox = new wxCheckBox(wid, id_render_light,
			wxT("&show light"));
		checkbox->SetValue(true);
		box->Add(checkbox);

		checkbox = new wxCheckBox(wid, id_render_sound,
			wxT("&show sound"));
		checkbox->SetValue(true);
		{
			wxButton *checkbox;
			/*
					checkbox = new wxButton(wid, id_reload_all_resouce,
					wxT("reload all resource"));
					box->Add(checkbox);
					checkbox = new wxButton(wid, id_unloadUnreferencedResources,
					wxT("unload unreferenced resources"));
					//		checkbox->SetValue(true);
					box->Add(checkbox);*/


			checkbox = new wxButton(wid, id_render_solid,
				wxT("render solid"));
			//		checkbox->SetValue(true);
			box->Add(checkbox);

			checkbox = new wxButton(wid, id_render_wireframe,
				wxT("render wireframe"));
			//checkbox->SetValue(false);
			box->Add(checkbox);

			checkbox = new wxButton(wid, id_render_point,
				wxT("render point"));
			//checkbox->SetValue(false);
			box->Add(checkbox);
		}


		box->Fit(wid);
		box->Layout();
		wid->SetSizer(box);
		frame->getToolBook()->AddPage(wid, L"render option");
	}
	//##ModelId=44BA0692038A
	void renderOptionTool_t::onButtonEvent(wxCommandEvent &event)
	{
		if (event.GetId() == id_render_in_game_mode){
			GetEditor()->GetEditorScene()->enableRenderOptimize(event.IsChecked());
		}
		else if (event.GetId() == id_render_sector){

		}
		else if (event.GetId() == id_render_light){

		}
		else if (event.GetId() == id_render_sound){

		}
		else if (event.GetId() == id_render_mesh){

		}
		else if (event.GetId() == id_render_solid){
			Ogre::Camera *c = frame->getView()->getCamera();
			c->setPolygonMode(Ogre::PM_SOLID);
			//GetEditor()->GetEditorScene()->getDestinationRenderSystem()->_setPolygonMode(PolygonMode::PM_SOLID);
		}
		else if (event.GetId() == id_render_wireframe){
			Ogre::Camera *c = frame->getView()->getCamera();
			c->setPolygonMode(Ogre::PM_WIREFRAME);
			//GetEditor()->GetEditorScene()->getDestinationRenderSystem()->_setPolygonMode(PolygonMode::PM_WIREFRAME);
			//GetEditor()->GetEditorScene()->getDestinationRenderSystem()->_setCullingMode(CullingMode::CULL_CLOCKWISE);
		}
		else if (event.GetId() == id_render_point){
			Ogre::Camera *c = frame->getView()->getCamera();
			c->setPolygonMode(Ogre::PM_POINTS);
			//GetEditor()->GetEditorScene()->getDestinationRenderSystem()->_setPolygonMode(PolygonMode::PM_POINTS);
			//GetEditor()->GetEditorScene()->getDestinationRenderSystem()->_setCullingMode(CullingMode::CULL_ANTICLOCKWISE);
		}
		else if (event.GetId() == id_reload_all_resouce){
			Ogre::MeshManager::getSingleton().reloadAll();
			Ogre::MaterialManager::getSingleton().reloadAll();
			Ogre::TextureManager::getSingleton().reloadAll();
		}
		else if (event.GetId() == id_unloadUnreferencedResources){
			Ogre::MeshManager::getSingleton().unloadUnreferencedResources();
			Ogre::MaterialManager::getSingleton().unloadUnreferencedResources();
			Ogre::TextureManager::getSingleton().unloadUnreferencedResources();
		}
	}
}