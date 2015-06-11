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


	// main event receivers
	wxBEGIN_EVENT_TABLE(lightProp_t, wxPanel)
		EVT_PG_CHANGED(PGID, lightProp_t::OnPropertyGridChange)
		wxEND_EVENT_TABLE();

	bool lightProp_t::init()
	{
		Create(frame->getPropBook(), -1, wxPoint(0, 0), wxSize(400, 400));

		// Column
		topSizer = new wxBoxSizer(wxVERTICAL);

		propGrid =
			new wxPropertyGridManager(this,
			// Don't change this into wxID_ANY in the sample, or the
			// event handling will obviously be broken.
			PGID,
			wxDefaultPosition,
			wxDefaultSize,
			wxPG_BOLD_MODIFIED |
			wxPG_SPLITTER_AUTO_CENTER |
			wxPG_AUTO_SORT |
			wxPG_HIDE_MARGIN | wxPG_STATIC_SPLITTER |
			wxPG_TOOLTIPS |
			wxPG_LIMITED_EDITING |
			wxTAB_TRAVERSAL |
			wxPG_TOOLBAR |
			wxPG_DESCRIPTION
			);
		propGrid->SetExtraStyle(wxPG_EX_MODE_BUTTONS
			| wxPG_EX_HELP_AS_TOOLTIPS
			);

		propGrid->GetGrid()->SetVerticalSpacing(2);
		propGrid->AddPage(wxT("Standard Items"));
		propGrid->Append(new wxStringProperty(wxT("light type"), wxPG_LABEL, wxT("point light")));
		auto pgId = propGrid->Append(new wxColourProperty(wxT("diffuse"), wxPG_LABEL, *wxGREEN));
		pgId = propGrid->Append(new wxColourProperty(wxT("specular"), wxPG_LABEL, *wxGREEN));
		propGrid->Append(new wxFloatProperty(wxT("spotOuter"), wxPG_LABEL, 0.698132));
		propGrid->Append(new wxFloatProperty(wxT("spotInner"), wxPG_LABEL, 0.523599));
		propGrid->Append(new wxFloatProperty(wxT("spotFalloff"), wxPG_LABEL, 1));
		propGrid->Append(new wxFloatProperty(wxT("range"), wxPG_LABEL, 50));
		propGrid->Append(new wxFloatProperty(wxT("attenuationConst"), wxPG_LABEL, 1));
		propGrid->Append(new wxFloatProperty(wxT("attenuationLinear"), wxPG_LABEL, 0));
		propGrid->Append(new wxFloatProperty(wxT("attenuationQuad"), wxPG_LABEL, 0));
		propGrid->Append(new wxFloatProperty(wxT("powerScale"), wxPG_LABEL, 1));

		topSizer->Add(propGrid, 1, wxEXPAND);

		SetSizerAndFit(topSizer);
		topSizer->SetSizeHints(this);

		frame->getPropBook()->AddPage(this, wxT("lightProp"));
		index = frame->getPropBook()->GetPageCount() - 1;
		typeName = LightFactory::FACTORY_TYPE_NAME;

		return true;
	}

	lightProp_t::lightProp_t()
		:light(NULL)
	{
		topSizer = nullptr;
		propGrid = nullptr;
	}

	lightProp_t::~lightProp_t()
	{
		SAFE_DELETE(propGrid);
	}

	void lightProp_t::update(Ogre::MovableObject *o, bool to)
	{
		if (!o)
			return;

		if (o->getMovableType() == LightFactory::FACTORY_TYPE_NAME)
		{
			light = static_cast<Ogre::Light *>(o);
		}
		_update(light);
	}

	void lightProp_t::_update(Ogre::Light *_obj)
	{
		light = _obj;
		if (!light)
			return;

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

	void lightProp_t::update(float t)
	{
		Ogre::LightList list = GetEngine()->mGSceneMgr->_getLightsAffectingFrustum();
		Ogre::LightList::iterator it = list.begin();
		while (it != list.end())
		{
			_update(*it);
			it++;
		}
	}

	void lightProp_t::OnPropertyGridChange(wxPropertyGridEvent& event)
	{
		if (!light)
			return;
		wxColor c;
		ColourValue v;
		c = *((wxColour*)propGrid->GetProperty(wxT("diffuse")));
		v.r = c.Red() / 255.0f;
		v.g = c.Green() / 255.0f;
		v.b = c.Blue() / 255.0f;
		v.a = c.Alpha() / 255.0f;

		light->setDiffuseColour(v);
		c = *((wxColour*)propGrid->GetProperty(wxT("specular")));
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

	bool lightProp_t::Show(bool show)
	{
		return wxWindow::Show(show);
	}
}