#pragma once

inline const Ogre::ColourValue& ConvertFromPropertyValueToOgreColor(wxPGProperty*prop)
{
	Ogre::ColourValue &v = *new Ogre::ColourValue();
	wxAny value;
	wxColour col;

	value = prop->GetValue();
	col = value.As<wxColour>();

	v.r = col.Red() / 255;
	v.g = col.Green() / 255;
	v.b = col.Blue() / 255;
	v.a = col.Alpha() / 255;

	Debug("color %f %f %f %f", v.r, v.g, v.b, v.a);
	return v;
}