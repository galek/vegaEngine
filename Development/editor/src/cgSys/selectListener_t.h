#pragma once

namespace Ogre{
	class SceneNode;
	class MovableObject;
};
using namespace Ogre;


namespace vega
{
	class selectListener_t{
	public:
		virtual void onSelNode(SceneNode *me){};
		virtual void onSelMovable(MovableObject *me){}
	};
}