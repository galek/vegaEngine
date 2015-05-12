#pragma once
#include "OgreAny.h"


namespace vega
{
	class type_t :public Ogre::Any{
	public:
		std::string userData;
		const long typeId;
		const bool editable;
		const bool showInGame;
		const bool dummy;
		const bool saveable;
		type_t(const type_t &me) :typeId(me.typeId), editable(me.editable), showInGame(me.showInGame), dummy(me.dummy), saveable(me.saveable){}
		type_t(const int typeId, const bool editable, const bool showInGame, const bool dummy, const bool saveable) :typeId(typeId), editable(editable), showInGame(showInGame), dummy(dummy), saveable(saveable){}
		~type_t();
	};
}
#include "type.h"