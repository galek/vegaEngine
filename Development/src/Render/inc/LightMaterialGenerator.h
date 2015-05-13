#ifndef H_WJ_LightMaterialGenerator
#define H_WJ_LightMaterialGenerator

#include "MaterialGenerator.h"

namespace vega
{
	class LightMaterialGenerator : public MaterialGenerator
	{
	public:
		/** Permutation of light materials
		 */
		enum MaterialID
		{
			MI_POINT = 0x01, // Is a point light
			MI_SPOTLIGHT = 0x02, // Is a spot light
			MI_DIRECTIONAL = 0x04, // Is a directional light

			MI_ATTENUATED = 0x08, // Rendered attenuated
			MI_SPECULAR = 0x10, // Specular component is calculated
			MI_SHADOW_CASTER = 0x20  // Will cast shadows
		};

		LightMaterialGenerator();
		virtual ~LightMaterialGenerator();
	};
}
#endif