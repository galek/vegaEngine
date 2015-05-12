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
#include "lightProp_t.h"
#include "wx/choicebk.h"
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>


namespace vega
{
	static const long idP = wxNewId();
	static const long idR = wxNewId();
	static const long idRY = wxNewId();
	static const long idS = wxNewId();
	static const long PGID = wxNewId();

	BEGIN_EVENT_TABLE(lightProp_t, wxPanel)
		EVT_PG_CHANGED(PGID, lightProp_t::OnPropertyGridChange)
		END_EVENT_TABLE()

		bool lightProp_t::init()
	{
#if NICK
		wxWindow* panel = this;// new wxPanel(frame->getPropBook(),-1,wxPoint(0,0),wxSize(400,400));
		Create(frame->getPropBook(), -1, wxPoint(0, 0), wxSize(400, 400));

		// Column
		wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
		wxPropertyGridManager* pgman = propGrid =
			new wxPropertyGridManager(panel,
			// Don't change this into wxID_ANY in the sample, or the
			// event handling will obviously be broken.
			PGID, /*wxID_ANY*/
			wxDefaultPosition,
			wxDefaultSize,
			wxPG_BOLD_MODIFIED |
			wxPG_SPLITTER_AUTO_CENTER |
			wxPG_AUTO_SORT |
			//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
			//wxPG_TOOLTIPS |
			//wxPG_HIDE_CATEGORIES |
			//wxPG_LIMITED_EDITING |
			wxTAB_TRAVERSAL |
			wxPG_TOOLBAR |
			wxPG_DESCRIPTION
			//wxPG_COMPACTOR
			);

		pgman->SetExtraStyle(wxPG_EX_MODE_BUTTONS
			//| wxPG_EX_GREY_LABEL_WHEN_DISABLED
			//| wxPG_EX_NATIVE_DOUBLE_BUFFERING
			//| wxPG_EX_HELP_AS_TOOLTIPS
			);

		pgman->RegisterAdvancedPropertyClasses();
		propGrid->GetGrid()->SetVerticalSpacing(2);

		pgman->AddPage(wxT("Standard Items"));
		// Register all editors (SpinCtrl etc.)
		//propGrid->RegisterAdditionalEditors();
		propGrid->Append(&wxStringProperty(wxT("light type"), wxPG_LABEL, wxT("point light")));
		wxPGId pgId = propGrid->Append(&wxColourProperty(wxT("diffuse"), wxPG_LABEL, *wxGREEN));
		//propGrid->SetPropertyAttribute(pgId,wxPG_COLOUR_ALLOW_CUSTOM,(long)0);
		pgId = propGrid->Append(&wxColourProperty(wxT("specular"), wxPG_LABEL, *wxGREEN));
		//propGrid->SetPropertyAttribute(pgId,wxPG_COLOUR_ALLOW_CUSTOM,(long)0);
		propGrid->Append(&wxFloatProperty(wxT("spotOuter"), wxPG_LABEL, 0.698132));
		propGrid->Append(&wxFloatProperty(wxT("spotInner"), wxPG_LABEL, 0.523599));
		propGrid->Append(&wxFloatProperty(wxT("spotFalloff"), wxPG_LABEL, 1));
		propGrid->Append(&wxFloatProperty(wxT("range"), wxPG_LABEL, 50));
		propGrid->Append(&wxFloatProperty(wxT("attenuationConst"), wxPG_LABEL, 1));
		propGrid->Append(&wxFloatProperty(wxT("attenuationLinear"), wxPG_LABEL, 0));
		propGrid->Append(&wxFloatProperty(wxT("attenuationQuad"), wxPG_LABEL, 0));
		propGrid->Append(&wxFloatProperty(wxT("powerScale"), wxPG_LABEL, 1));

		topSizer->Add(propGrid, 1, wxEXPAND);
		panel->SetSizer(topSizer);
		topSizer->SetSizeHints(panel);

		frame->getPropBook()->AddPage(panel, wxT("lightProp"));
		index = frame->getPropBook()->GetPageCount() - 1;
		typeName = LightFactory::FACTORY_TYPE_NAME;
#endif
		return true;
	}

	lightProp_t::lightProp_t()
		:light(NULL){

	}

	void lightProp_t::update(Ogre::MovableObject *o, bool to){
		//Nick тут
		light = static_cast<Ogre::Light *>(o);
		if (light){
			wxColor c;
			Ogre::ColourValue v;
			v = light->getDiffuseColour();
			c.Set(v.r * 255, v.g * 255, v.b * 255, v.a * 255);
			propGrid->SetPropertyValue(wxT("diffuse"), c);
			v = light->getSpecularColour();
			c.Set(v.r * 255, v.g * 255, v.b * 255, v.a * 255);
			propGrid->SetPropertyValue(wxT("specular"), c);



			lightTypes = light->getType();
			if (lightTypes = Ogre::Light::LT_POINT)
				propGrid->SetPropertyValue(wxT("light type"), wxT("point light"));
			else if (lightTypes = Ogre::Light::LT_DIRECTIONAL)
				propGrid->SetPropertyValue(wxT("light type"), wxT("directional light"));
			else if (lightTypes = Ogre::Light::LT_SPOTLIGHT)
				propGrid->SetPropertyValue(wxT("light type"), wxT("spot light"));
			propGrid->SetPropertyValue(wxT("spotOuter"), light->getSpotlightOuterAngle().valueRadians());
			propGrid->SetPropertyValue(wxT("spotInner"), light->getSpotlightInnerAngle().valueRadians());
			propGrid->SetPropertyValue(wxT("spotFalloff"), light->getSpotlightFalloff());
			propGrid->SetPropertyValue(wxT("range"), light->getAttenuationRange());
			propGrid->SetPropertyValue(wxT("attenuationConst"), light->getAttenuationConstant());
			propGrid->SetPropertyValue(wxT("attenuationLinear"), light->getAttenuationLinear());
			propGrid->SetPropertyValue(wxT("attenuationQuad"), light->getAttenuationQuadric());
			propGrid->SetPropertyValue(wxT("powerScale"), light->getPowerScale());
		}

	}

	void lightProp_t::OnPropertyGridChange(wxPropertyGridEvent& event)
	{
#if 0
		/*const wxString& name = event.GetPropertyName();
		wxVariant value = event.GetPropertyValue();*/
		if (light){
			wxColor c;
			ColourValue v;
			c = *((wxColour*)propGrid->GetPropertyValueAsWxObjectPtr(wxT("diffuse")));
			v.r = c.Red() / 255.0f;
			v.g = c.Green() / 255.0f;
			v.b = c.Blue() / 255.0f;
			v.a = c.Alpha() / 255.0f;

			light->setDiffuseColour(v);
			c = *((wxColour*)propGrid->GetPropertyValueAsWxObjectPtr(wxT("specular")));
			v.r = c.Red() / 255.0f;
			v.g = c.Green() / 255.0f;
			v.b = c.Blue() / 255.0f;
			v.a = c.Alpha() / 255.0f;
			light->setSpecularColour(v);
			if (lightTypes == Light::LT_SPOTLIGHT)
				light->setSpotlightRange(
				Radian(propGrid->GetPropertyValueAsDouble(wxT("spotInner"))),
				Radian(propGrid->GetPropertyValueAsDouble(wxT("spotOuter"))),
				propGrid->GetPropertyValueAsDouble(wxT("spotFalloff"))
				);
			light->setAttenuation(propGrid->GetPropertyValueAsDouble(wxT("range")),
				propGrid->GetPropertyValueAsDouble(wxT("attenuationConst")),
				propGrid->GetPropertyValueAsDouble(wxT("attenuationLinear")),
				propGrid->GetPropertyValueAsDouble(wxT("attenuationQuad")));
			light->setPowerScale(propGrid->GetPropertyValueAsDouble(wxT("powerScale")));
		}
#endif
	}
	bool lightProp_t::Show(bool show){
		if (show)
			light = NULL;
		else
			light = NULL;
		return wxWindow::Show(show);
	}
}