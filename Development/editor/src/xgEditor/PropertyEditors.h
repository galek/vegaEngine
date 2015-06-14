#pragma once

namespace vega
{
	//EditorParts
	class LightPropertyEditor;
	//
	class PropertyEditors 
	{
	public:
		PropertyEditors();
	public:
		LightPropertyEditor*m_LightPE;
	};

	PropertyEditors& GetPropEditors();
}