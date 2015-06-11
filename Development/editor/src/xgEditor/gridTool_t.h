#pragma once
namespace Ogre{
	class ManualObject;
};

namespace vega
{
	class gridTool_t : public tool_t
	{
	public:
		~gridTool_t();
		virtual bool init();
		virtual bool onUiEvent(wxEvent & event);
	private:
		Ogre::ManualObject *grid;
	};
}