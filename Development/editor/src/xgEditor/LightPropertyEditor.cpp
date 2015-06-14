#include "stdafx.h"
#include "moveObjectProp_t.h"
#include "frame_t.h"
#include "LightPropertyEditor.h"
#include "wx/choicebk.h"

namespace vega
{
	static const long idP = wxNewId();
	static const long idR = wxNewId();
	static const long idRY = wxNewId();
	static const long idS = wxNewId();
	static const long PGID = wxNewId();


	// main event receivers
	wxBEGIN_EVENT_TABLE(LightPropertyEditor, wxPanel)
		EVT_PG_CHANGED(PGID, LightPropertyEditor::OnPropertyGridChange)
		wxEND_EVENT_TABLE();

	bool LightPropertyEditor::init()
	{
		// Сделаем разделитель
		Create(frame->getPropBook(), -1, wxPoint(0, 0), wxSize(400, 400));

		// Сделаем регулятор размера
		m_TopSizer = new wxBoxSizer(wxVERTICAL);

		m_PropGrid =
			new wxPropertyGridManager(this,
				// Don't change this into wxID_ANY in the sample, or the
				// event handling will obviously be broken.
				PGID,
				wxDefaultPosition,
				wxDefaultSize,
				wxPG_BOLD_MODIFIED |
				wxPG_SPLITTER_AUTO_CENTER |
				wxPG_AUTO_SORT |
				wxPG_TOOLTIPS |
				wxTAB_TRAVERSAL |
				wxPG_DESCRIPTION
				);
		m_PropGrid->SetExtraStyle(wxPG_EX_MODE_BUTTONS
			| wxPG_EX_HELP_AS_TOOLTIPS
			);


		// Включаем категории
		m_PropGrid->EnableCategories(true);
		// Задаем элементы
		m_PropGrid->GetGrid()->SetVerticalSpacing(2);

		// Main Settings category
		m_PropGrid->Append(new wxPropertyCategory("Main"));

		wxArrayString mLightTypes;
		mLightTypes.Add("Point Light");
		mLightTypes.Add("Spot Light");
		mLightTypes.Add("Directional Light");
		m_PropGrid->Append(new wxEnumProperty("light type",
			wxPG_LABEL,
			mLightTypes));

		m_PropGrid->Append(new wxFloatProperty(wxT("powerScale"), wxPG_LABEL, 1));
		m_PropGrid->Append(new wxFloatProperty(wxT("attenuationConst"), wxPG_LABEL, 1));
		m_PropGrid->Append(new wxFloatProperty(wxT("attenuationLinear"), wxPG_LABEL, 0));
		m_PropGrid->Append(new wxFloatProperty(wxT("attenuationQuad"), wxPG_LABEL, 0));
		m_PropGrid->Append(new wxFloatProperty(wxT("spotOuter"), wxPG_LABEL, 0.698132));
		m_PropGrid->Append(new wxFloatProperty(wxT("spotInner"), wxPG_LABEL, 0.523599));
		m_PropGrid->Append(new wxFloatProperty(wxT("spotFalloff"), wxPG_LABEL, 1));
		m_PropGrid->Append(new wxFloatProperty(wxT("range"), wxPG_LABEL, 50));
		// Color Settings category
		m_PropGrid->Append(new wxPropertyCategory("Color Settings"));
		m_PropGrid->Append(new wxColourProperty(wxT("diffuse"), wxPG_LABEL, *wxGREEN));
		m_PropGrid->Append(new wxColourProperty(wxT("specular"), wxPG_LABEL, *wxGREEN));

		m_TopSizer->Add(m_PropGrid, 1, wxEXPAND);

		SetSizerAndFit(m_TopSizer);
		m_TopSizer->SetSizeHints(this);

		frame->getPropBook()->AddPage(this, wxT("lightProp"));
		index = frame->getPropBook()->GetPageCount() - 1;
		typeName = LightFactory::FACTORY_TYPE_NAME;

		return true;
	}

	LightPropertyEditor::LightPropertyEditor()
	{
		m_EditableLight = nullptr;
		m_TopSizer = nullptr;
		m_PropGrid = nullptr;
	}

	LightPropertyEditor::~LightPropertyEditor()
	{
		SAFE_DELETE(m_PropGrid);
	}

	void LightPropertyEditor::_update()
	{
		if (!m_EditableLight)
			return;

		if (m_PropGrid->IsPropertyModified(wxT("diffuse")) || m_PropGrid->IsPropertyModified(wxT("specular")))
		{
			wxColor c;
			Ogre::ColourValue v;
			v = m_EditableLight->getDiffuse();
			c.Set(v.r * 255, v.g * 255, v.b * 255, v.a * 255);
			m_PropGrid->SetPropertyValue(wxT("diffuse"), c);
			v = m_EditableLight->getSpecular();
			c.Set(v.r * 255, v.g * 255, v.b * 255, v.a * 255);
			m_PropGrid->SetPropertyValue(wxT("specular"), c);
		}

		if (m_PropGrid->IsPropertyModified(wxT("light type")))
		{
			lightTypes = m_EditableLight->GetLightType();
			if (lightTypes = ActorLight::POINT)
				m_PropGrid->SetPropertyValue(wxT("light type"), wxT("point light"));
			else if (lightTypes = ActorLight::DIRECTIONAL)
				m_PropGrid->SetPropertyValue(wxT("light type"), wxT("directional light"));
			else if (lightTypes = ActorLight::SPOT)
				m_PropGrid->SetPropertyValue(wxT("light type"), wxT("spot light"));
		}
		if (m_PropGrid->IsPropertyModified(wxT("spotOuter")))
			m_PropGrid->SetPropertyValue(wxT("spotOuter"), m_EditableLight->GetSpotOuter());
		if (m_PropGrid->IsPropertyModified(wxT("spotInner")))
			m_PropGrid->SetPropertyValue(wxT("spotInner"), m_EditableLight->GetSpotInner());
		if (m_PropGrid->IsPropertyModified(wxT("spotFalloff")))
			m_PropGrid->SetPropertyValue(wxT("spotFalloff"), m_EditableLight->GetSpotlightFalloff());
		if (m_PropGrid->IsPropertyModified(wxT("range")))
			m_PropGrid->SetPropertyValue(wxT("range"), m_EditableLight->GetAttenuationRange());
		if (m_PropGrid->IsPropertyModified(wxT("attenuationConst")))
			m_PropGrid->SetPropertyValue(wxT("attenuationConst"), m_EditableLight->GetAttenuationConstant());
		if (m_PropGrid->IsPropertyModified(wxT("attenuationLinear")))
			m_PropGrid->SetPropertyValue(wxT("attenuationLinear"), m_EditableLight->GetAttenuationLinear());
		if (m_PropGrid->IsPropertyModified(wxT("attenuationQuad")))
			m_PropGrid->SetPropertyValue(wxT("attenuationQuad"), m_EditableLight->GetAttenuationQuadric());
		if (m_PropGrid->IsPropertyModified(wxT("powerScale")))
			m_PropGrid->SetPropertyValue(wxT("powerScale"), m_EditableLight->GetPowerScale());
	}

	void LightPropertyEditor::update(float t)
	{
		/*Ogre::LightList list = GetEngine()->mGSceneMgr->_getLightsAffectingFrustum();
		Ogre::LightList::iterator it = list.begin();
		while (it != list.end())
		{
			_update((ActorLight*)(*it));
			it++;
		}*/
		_update();
	}

	void LightPropertyEditor::OnPropertyGridChange(wxPropertyGridEvent& _event)
	{
		if (!m_EditableLight)
			return;

		auto m_changed = _event.GetPropertyName();
		auto m_property = /*m_PropGrid->GetProperty(m_changed)*/_event.GetProperty();
		if (m_changed == wxT("diffuse"))
		{
			Debug("CHANGED diffuse");
			auto color = ConvertFromPropertyValueToOgreColor(m_property);
			m_EditableLight->setDiffuse(color);
		}
		else if (m_changed == wxT("specular"))
		{
			Debug("CHANGED specular");
			auto color = ConvertFromPropertyValueToOgreColor(m_property);
			m_EditableLight->setSpecular(color);
		}
		TODO("UNCOMMENT THIS AND FINISH")
			/*if (lightTypes == Light::LT_SPOTLIGHT)
				light->setSpotlightRange(
					Radian(m_PropGrid->GetPropertyValueAsDouble(wxT("spotInner"))),
					Radian(m_PropGrid->GetPropertyValueAsDouble(wxT("spotOuter"))),
					m_PropGrid->GetPropertyValueAsDouble(wxT("spotFalloff"))
					);
			light->setAttenuation(m_PropGrid->GetPropertyValueAsDouble(wxT("range")),
				m_PropGrid->GetPropertyValueAsDouble(wxT("attenuationConst")),
				m_PropGrid->GetPropertyValueAsDouble(wxT("attenuationLinear")),
				m_PropGrid->GetPropertyValueAsDouble(wxT("attenuationQuad")));
			light->setPowerScale(m_PropGrid->GetPropertyValueAsDouble(wxT("powerScale")));*/
	}

	bool LightPropertyEditor::Show(bool show)
	{
		return wxWindow::Show(show);
	}

	void LightPropertyEditor::SetEditableLight(ActorLight * _mEditableLight)
	{
		if (!_mEditableLight)
			return;

		m_EditableLight = _mEditableLight;
	}

	void LightPropertyEditor::SetEditableLightIsNull()
	{
		m_EditableLight = nullptr;
	}
}