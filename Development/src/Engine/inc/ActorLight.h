/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class ENGINE_API ActorLight :public Actor
	{
	public:
		enum _ltype
		{
			POINT = 0,
			DIRECTIONAL = 1,
			SPOT = 2
		};
		// !@Creating Lighting(string,SceneNode,Vector3,Vector3)
		static ActorLight*PointLight(std::string _mName);
		// !@Creating Lighting(string,SceneNode,Vector3,Vector3)
		static ActorLight*DirectLight(std::string _mName);
		// !@Creating Lighting(string,SceneNode,Vector3,Vector3)
		static ActorLight*SpotLight(std::string _mName);
		// !@Creating Lighting(string,SceneNode,Vector3,Vector3)
		ActorLight(std::string _mName, _ltype _type, Ogre::Vector3 _pos = Ogre::Vector3(0, 0, 0), Ogre::Vector3 _rot = Ogre::Vector3::UNIT_X);
		virtual ~ActorLight();
		// !@Attaching to Actor
		void attach(Actor *_mParent);
		// !@Set Type(int).1-DIRECTIONAL,2-SPOT,else-POINT
		void setType(_ltype _type);
		// !@Set Cast Shadows(bool)
		void setCastShadows(bool _status = true);
		// !@Set Visible(bool)
		void setVisible(bool _status = true);
		// !@Set Position(Ogre::Vector3)
		void setPosition(const Ogre::Vector3& _pos);
		// !@Set Direction(Ogre::Vector3)
		void setDirection(const Ogre::Vector3& _quat);
		// !@Set LightNode Scale(Ogre::Vector3)
		void setScale(const Ogre::Vector3& _scale);
		// !@Set Lighting Power(float,float,float,float)
		void setPowerScale(float _range, float _constant = 0.0f, float _linear = 1.0f, float _quadratic = 0.0f);
		// !@Set Diffuse Color(float,float,float)
		void setDiffuse(float _r, float _g, float _b);
		// !@Set Specular Color(float,float,float)
		void setSpecular(float _r, float _g, float _b);
		// !@Set Diffuse Color(float,float,float)
		void setDiffuse(const Ogre::ColourValue&);
		// !@Set Specular Color(float,float,float)
		void setSpecular(const Ogre::ColourValue&);
		// !@Set Shadow FarClip Distance(float)
		void setShadowFarClipDistance(float _d);
		// !@Set Shadow Far Distance(float)
		void setShadowFarDistance(float _d);
		// !@Get Direction
		const Ogre::Vector3& getDirection();
		// !@Get Diffuse Color()
		const Ogre::ColourValue& getDiffuse();
		// !@Get Specular Color()
		const Ogre::ColourValue& getSpecular();
	public:
		// !@Method for Editor Fabric-editor only!
		void _updateRenderQueue(Ogre::RenderQueue *queue);
		// !@Method for Editor Fabric-editor only!
		const Ogre::AxisAlignedBox& getBoundingBox(void) const;
		__inline Ogre::Light * getOgreLight(void) const{ return mLight; }
	private:
		// !@Pointer to a Wire Bounding Box for this Node,used in edit mode
		Ogre::WireBoundingBox *mRangeBox;
	private:
		Ogre::Light *mLight;
		std::string mName;
		_ltype mLightType;
	};
}