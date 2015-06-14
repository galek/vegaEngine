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

	enum type_t_enum
	{
		mesh_type,
		entity_type,
		fx_type,
		light_type,
		grid_helper_type,
		axis_helper_type,
		sector_type,
		ai_navi_type,
		helper_mesh_type
	};
	extern type_t mesh_type_;
	extern type_t entity_type_;
	extern type_t fx_type_;
	extern type_t light_type_;
	extern type_t grid_helper_type_;
	extern type_t axis_helper_type_;
	extern type_t sector_type_;
	extern type_t ai_navi_type_;
	extern type_t dummy_type_;
}