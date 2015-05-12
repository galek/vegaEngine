#ifndef _EXMATERIALSET_H
#define _EXMATERIALSET_H

#include "ExShader.h"
#include "ExTools.h"
#include "ExPrerequisites.h"
#include "VMaxLog.h"

namespace VMax
{
	class ExMaterialSet
	{
	public:
	private:
		std::vector<ExMaterial*> m_materials;
		std::vector<std::string> m_textures;
		std::vector<ExShader*> m_Shaders;
		ExMaterial* m_default;
	protected:

	public:
		//constructor
		ExMaterialSet(ExOgreConverter* converter);

		//destructor
		~ExMaterialSet();

		//clear
		void clear();

		bool getTextureSameFileNameExist(std::string filepath, std::string name);

		std::string getUniqueTextureName(std::string filepath);

		ExMaterial* getMaterialByName(std::string name);

		//add material
		void addMaterial(ExMaterial* pMat = 0);

		//get material
		ExMaterial* getMaterial(IGameMaterial* pGameMaterial);

		//write materials to Ogre Script
		bool writeOgreScript(ParamList &params);
	private:
		ExShader* getShader(std::string& name);
		void addShader(ExShader* shader);
		ExShader* createShader(ExMaterial* mat, ExShader::ShaderType type, ParamList &params);
	protected:
	};

};	//end namespace

#endif
