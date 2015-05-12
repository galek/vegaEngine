#pragma once


namespace vega
{
	enum type_t_enum{
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