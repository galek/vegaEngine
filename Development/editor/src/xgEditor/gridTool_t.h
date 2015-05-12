#pragma once
namespace Ogre{
	class ManualObject;
};

namespace vega
{
	class gridTool_t : public tool_t{
		Ogre::ManualObject *grid;
	public:
		~gridTool_t();
		virtual bool init();
		//##ModelId=44B8E4980203
		virtual bool onUiEvent(wxEvent & event);
	};
}