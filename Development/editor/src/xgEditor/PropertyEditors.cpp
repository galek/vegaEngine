#include "stdafx.h"
#include "PropertyEditors.h"

namespace vega
{
	static PropertyEditors peditors;

	PropertyEditors& GetPropEditors()
	{
		return peditors;
	}

	PropertyEditors::PropertyEditors()
	{
		m_LightPE = nullptr;
	}
}