#ifndef _EXSCENE_H
#define _EXSCENE_H


#include "ExOgreConverter.h"

namespace VMax
{
	TODO("”дали эти типы,и замени из наших актеры")
	enum ExOgreLightType
	{
		OGRE_LIGHT_POINT,
		OGRE_LIGHT_DIRECTIONAL,
		OGRE_LIGHT_SPOT,
		OGRE_LIGHT_RADPOINT
	};

	struct EasyOgrePoint3
	{
		float x, y, z;
		EasyOgrePoint3()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		EasyOgrePoint3(float X, float Y, float Z)
		{
			x = X;
			y = Y;
			z = Z;
		}
	};

	struct ExOgrePoint4
	{
		float w, x, y, z;
		ExOgrePoint4()
		{
			w = 0.0f;
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		ExOgrePoint4(float W, float X, float Y, float Z)
		{
			w = W;
			x = X;
			y = Y;
			z = Z;
		}
	};


	class ExScene
	{
	public:
		//constructor
		ExScene(ExOgreConverter* converter);

		//destructor
		~ExScene();

		TiXmlElement* writeNodeData(TiXmlElement* parent, IGameNode* pGameNode, IGameObject::ObjectTypes type);
		TiXmlElement* writeEntityData(TiXmlElement* parent, IGameNode* pGameNode, IGameMesh* pGameMesh, std::vector<ExMaterial*> lmat);
		TiXmlElement* writeCameraData(TiXmlElement* parent, IGameCamera* pGameCamera);
		TiXmlElement* writeLightData(TiXmlElement* parent, IGameLight* pGameLight);

		bool writeSceneFile();
	protected:
		int id_counter;
		ExOgreConverter* m_converter;
		std::string scenePath;
		TiXmlDocument* xmlDoc;
		TiXmlElement *sceneElement;
		TiXmlElement *nodesElement;

		void initXmlDocument();

		std::string getLightTypeString(ExOgreLightType type);
		std::string getBoolString(bool value);
		bool exportNodeAnimation(TiXmlElement* pAnimsElement, IGameNode* pGameNode, Interval animRange, std::string name, bool resample, IGameObject::ObjectTypes type);
	};
} // end of namespace
#endif
