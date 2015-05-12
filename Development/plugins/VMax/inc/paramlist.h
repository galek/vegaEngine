#ifndef PARAMLIST_H
#define PARAMLIST_H

#include "ExPrerequisites.h"

namespace VMax
{
	class ExSubEntity;

	enum TangentSemantic
	{
		TS_TEXCOORD,
		TS_TANGENT
	};

	enum ShaderMode
	{
		SHADER_NONE,
		SHADER_BUMP,
		SHADER_ALL
	};

	/***** Class ParamList *****/
	class ParamList
	{
	public:
		// class members
		bool exportMesh, exportMaterial, exportCameras, exportLights, lightingOff, exportAll,
			exportVertNorm, exportVertCol, exportSkeleton, exportSkelAnims, exportVertAnims, exportPoses,
			useSharedGeom, copyTextures, tangentsSplitMirrored, tangentsSplitRotated, tangentsUseParity,
			buildTangents, buildEdges, resampleAnims, yUpAxis, exportScene, generateLOD, convertToDDS;

		float lum;	// Length Unit Multiplier

		std::string outputDir, meshOutputDir, materialOutputDir, programOutputDir, texOutputDir, sceneFilename, resPrefix;

		std::vector<std::string> writtenMaterials;

		TangentSemantic tangentSemantic;

		ShaderMode exportProgram;

		std::vector<INode*> currentRootJoints;

		unsigned int maxTextureSize;

		int maxMipmaps;

		// constructor
		ParamList()	{
			lum = 1.0f;
			exportMesh = true;
			exportMaterial = true;
			exportProgram = SHADER_BUMP;
			exportSkeleton = true;
			exportSkelAnims = true;
			exportVertAnims = true;
			exportPoses = true;
			exportCameras = true;
			exportLights = true;
			exportAll = true;
			exportVertNorm = true;
			exportVertCol = true;
			lightingOff = false;
			useSharedGeom = false;
			copyTextures = true;

			convertToDDS = false;
			maxTextureSize = 2048;
			maxMipmaps = -1;
			resampleAnims = false;
			generateLOD = false;

			outputDir = "";
			meshOutputDir = "";
			materialOutputDir = "";
			programOutputDir = "";
			texOutputDir = "";
			sceneFilename = "";
			resPrefix = "";

			buildEdges = true;
			buildTangents = true;
			tangentsSplitMirrored = false;
			tangentsSplitRotated = false;
			tangentsUseParity = false;
			tangentSemantic = TS_TANGENT;
			currentRootJoints.clear();
			yUpAxis = true;
			exportScene = true;
		}

		ParamList& operator=(ParamList& source)
		{
			lum = source.lum;
			exportMesh = source.exportMesh;
			exportMaterial = source.exportMaterial;
			exportProgram = source.exportProgram;
			exportSkeleton = source.exportSkeleton;
			exportSkelAnims = source.exportSkelAnims;
			exportVertAnims = source.exportVertAnims;
			exportPoses = source.exportPoses;
			exportCameras = source.exportCameras;
			exportLights = source.exportLights;
			exportAll = source.exportAll;
			exportVertNorm = source.exportVertNorm;
			exportVertCol = source.exportVertCol;
			useSharedGeom = source.useSharedGeom;
			copyTextures = source.copyTextures;
			lightingOff = source.lightingOff;

			convertToDDS = source.convertToDDS;
			maxTextureSize = source.maxTextureSize;
			maxMipmaps = source.maxMipmaps;
			resampleAnims = source.resampleAnims;
			generateLOD = source.generateLOD;

			outputDir = source.outputDir;
			meshOutputDir = source.meshOutputDir;
			materialOutputDir = source.materialOutputDir;
			programOutputDir = source.programOutputDir;
			texOutputDir = source.texOutputDir;
			sceneFilename = source.sceneFilename;
			resPrefix = source.resPrefix;

			buildEdges = source.buildEdges;
			buildTangents = source.buildTangents;
			tangentsSplitMirrored = source.tangentsSplitMirrored;
			tangentsSplitRotated = source.tangentsSplitRotated;
			tangentsUseParity = source.tangentsUseParity;
			tangentSemantic = source.tangentSemantic;
			yUpAxis = source.yUpAxis;
			exportScene = source.exportScene;
			return *this;
		}

		// destructor
		~ParamList()
		{
		}
	};
}	//end namespace
#endif
