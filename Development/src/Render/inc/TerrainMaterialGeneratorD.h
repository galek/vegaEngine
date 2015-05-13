/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once
#include "renderdllex.h"

namespace vega
{

	/** \addtogroup Optional Components
	*  @{
	*/
	/** \addtogroup Terrain
	*  Some details on the terrain component
	*  @{
	*/

	/** A TerrainMaterialGenerator which can cope with normal mapped, specular mapped
		terrain.
		@note Requires the Cg plugin to render correctly
		*/
	class RENDER_API TerrainMaterialGeneratorD : public Ogre::TerrainMaterialGenerator
	{
	public:
		TerrainMaterialGeneratorD();
		~TerrainMaterialGeneratorD();

		/** Shader model 2 profile target.
		*/
		class SM2Profile : public Ogre::TerrainMaterialGenerator::Profile
		{
		public:
			SM2Profile(Ogre::TerrainMaterialGenerator* parent, const std::string& name, const std::string& desc);
			~SM2Profile();
			Ogre::MaterialPtr generate(const Ogre::Terrain* terrain);
			Ogre::MaterialPtr generateForCompositeMap(const Ogre::Terrain* terrain);
			Ogre::uint8 getMaxLayers(const Ogre::Terrain* terrain) const;
			void updateParams(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain);
			void updateParamsForCompositeMap(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain);
			void requestOptions(Ogre::Terrain* terrain);
			bool isVertexCompressionSupported() const;

			/** Whether to support a global colour map over the terrain in the shader,
				if it's present (default true).
				*/
			bool isGlobalColourMapEnabled() const  { return mGlobalColourMapEnabled; }
			/** Whether to support a global colour map over the terrain in the shader,
			if it's present (default true).
			*/
			void setGlobalColourMapEnabled(bool enabled);
			/** Whether to support a light map over the terrain in the shader,
			if it's present (default true).
			*/
			bool isLightmapEnabled() const  { return mLightmapEnabled; }
			/** Whether to support a light map over the terrain in the shader,
			if it's present (default true).
			*/
			void setLightmapEnabled(bool enabled);
			/** Whether to use the composite map to provide a lower LOD technique
				in the distance (default true).
				*/
			bool isCompositeMapEnabled() const  { return mCompositeMapEnabled; }
			/** Whether to use the composite map to provide a lower LOD technique
			in the distance (default true).
			*/
			void setCompositeMapEnabled(bool enabled);

			/// Internal
			bool _isSM3Available() const { return mSM3Available; }
			bool _isSM4Available() const { return mSM4Available; }

		protected:

			enum TechniqueType
			{
				HIGH_LOD,
				LOW_LOD,
				RENDER_COMPOSITE_MAP
			};
			void addTechnique(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain, TechniqueType tt);

			/// Interface definition for helper class to generate shaders
			class ShaderHelper : public Ogre::TerrainAlloc
			{
			public:
				ShaderHelper() {}
				virtual ~ShaderHelper() {}
				virtual Ogre::HighLevelGpuProgramPtr generateVertexProgram(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt);
				virtual Ogre::HighLevelGpuProgramPtr generateFragmentProgram(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt);
				virtual void updateParams(const SM2Profile* prof, const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain, bool compositeMap);
			protected:
				virtual std::string getVertexProgramName(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt);
				virtual std::string getFragmentProgramName(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt);
				virtual Ogre::HighLevelGpuProgramPtr createVertexProgram(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt) = 0;
				virtual Ogre::HighLevelGpuProgramPtr createFragmentProgram(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt) = 0;
				virtual void generateVertexProgramSource(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream);
				virtual void generateFragmentProgramSource(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream);
				virtual void generateVpHeader(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream) = 0;
				virtual void generateFpHeader(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream) = 0;
				virtual void generateVpLayer(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, unsigned int layer, Ogre::StringStream& outStream) = 0;
				virtual void generateFpLayer(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, unsigned int layer, Ogre::StringStream& outStream) = 0;
				virtual void generateVpFooter(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream) = 0;
				virtual void generateFpFooter(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream) = 0;
				virtual void defaultVpParams(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, const Ogre::HighLevelGpuProgramPtr& prog);
				virtual void defaultFpParams(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, const Ogre::HighLevelGpuProgramPtr& prog);
				virtual void updateVpParams(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, const Ogre::GpuProgramParametersSharedPtr& params);
				virtual void updateFpParams(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, const Ogre::GpuProgramParametersSharedPtr& params);
				static std::string getChannel(unsigned int idx);

			};

			/// Utility class to help with generating shaders for Cg / HLSL.
			class ShaderHelperCg : public ShaderHelper
			{
			protected:
				Ogre::HighLevelGpuProgramPtr createVertexProgram(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt);
				Ogre::HighLevelGpuProgramPtr createFragmentProgram(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt);
				void generateVpHeader(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream);
				void generateFpHeader(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream);
				void generateVpLayer(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, unsigned int layer, Ogre::StringStream& outStream);
				void generateFpLayer(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, unsigned int layer, Ogre::StringStream& outStream);
				void generateVpFooter(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream);
				void generateFpFooter(const SM2Profile* prof, const Ogre::Terrain* terrain, TechniqueType tt, Ogre::StringStream& outStream);
			};

			ShaderHelper* mShaderGen;
			bool mGlobalColourMapEnabled;
			bool mLightmapEnabled;
			bool mCompositeMapEnabled;
			bool mSM3Available;
			bool mSM4Available;
		};




	};



	/** @} */
	/** @} */


}
