#pragma once
#include "tool_t.h"
#include "update_t.h"

namespace vega
{
	class moveObjectProp_t :public tool_t, public update_t{
	protected:
		std::string typeName;
		int index;
	public:
		virtual std::string &getTypeName(){ return typeName; }
		virtual int getIndex(){ return index; }
		virtual void update(Ogre::MovableObject *o, bool to){}//update to object
	};
}