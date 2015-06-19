#pragma once

namespace Ogre{
	class SceneNode;
	class MovableObject;
};

namespace vega
{
	class selectListener_t
	{
	public:
		virtual void onSelNode(Actor *me){};
		virtual void onSelMovable(Ogre::MovableObject *me){}
	};
}