/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
// Last Update:15.01.13
// BugFix:изменил функцию установки типа света,добавил функцию добавления дальности прорисовки
#include "EnginePrivate.h"
#include "ActorLight.h"
#include "OgreWireBoundingBox.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	ActorLight::ActorLight(std::string _mName, _ltype _type, Ogre::Vector3 _pos, Ogre::Vector3 _rot)
	{
		mName = "ActorLight_" + _mName;

		mLight = GetEngine()->mGSceneMgr->createLight(mName);
		mNode = GetEngine()->mGSceneMgr->getRootSceneNode()->createChildSceneNode(mName);

		attach(this);

		//Ставим изначальные параметры
		setType(_type);
		setPosition(_pos);
		setDirection(_rot.normalisedCopy());
		setCastShadows(true);

		mRangeBox = new Ogre::WireBoundingBox();
		if (GetEngine()->isEditor()) {
			mRangeBox->setMaterial("LightMesh");
			Ogre::MaterialPtr baseWhiteNoLighting;
			if (Ogre::MaterialManager::getSingleton().resourceExists(_mName)){
				baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().getByName(_mName);
			}
			else{
				baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().create(_mName, Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
			}
			baseWhiteNoLighting->setLightingEnabled(true);

			baseWhiteNoLighting->setAmbient(0.8, 0.8, 0.0);
			baseWhiteNoLighting->setDiffuse(1, 1, 1, 1);
		}
	}

	//-------------------------------------------------------------------------------------
	ActorLight* ActorLight::PointLight(std::string _mName)
	{
		return new ActorLight(_mName, POINT);
	}

	//-------------------------------------------------------------------------------------
	ActorLight* ActorLight::DirectLight(std::string _mName)
	{
		return new ActorLight(_mName, DIRECTIONAL);
	}

	//-------------------------------------------------------------------------------------
	ActorLight* ActorLight::SpotLight(std::string _mName)
	{
		return new ActorLight(_mName, SPOT);
	}

	//-------------------------------------------------------------------------------------
	ActorLight::~ActorLight()
	{
		SAFE_DELETE(mRangeBox);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setShadowFarClipDistance(float _d)	{
		mLight->setShadowFarClipDistance(_d);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setShadowFarDistance(float _d)	{
		mLight->setShadowFarDistance(_d);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::attach(Actor *_mParent)	{
		_mParent->getNode()->attachObject(mLight);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setType(_ltype _type)
	{
		if (mLightType != _type)
		{
			mLightType = (_type);
			mLight->setType(Ogre::Light::LightTypes(_type));
		}
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setVisible(bool _status)	{
		mLight->setVisible(_status);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setCastShadows(bool _status)	{
		mLight->setCastShadows(_status);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setPosition(const Ogre::Vector3& _pos)	{
		mLight->setPosition(_pos);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDirection(const Ogre::Vector3& _quat)	{
		mLight->setDirection(_quat);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDiffuse(float _r, float _g, float _b)	{
		mLight->setDiffuseColour(_r, _g, _b);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setSpecular(float _r, float _g, float _b)	{
		mLight->setSpecularColour(_r, _g, _b);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setScale(const Ogre::Vector3& _scale) {
		if (mNode)
			mNode->setScale(_scale);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setPowerScale(float _range, float _constant, float _linear, float _quadratic)	{
		mLight->setAttenuation(_range, _constant, _linear, _quadratic);
	}

	//-------------------------------------------------------------------------------------
	const Ogre::Vector3& ActorLight::getDirection() {
		return mLight->getDirection();
	}

	//-------------------------------------------------------------------------------------
	const Ogre::ColourValue& ActorLight::getDiffuse() {
		return mLight->getDiffuseColour();
	}

	//-------------------------------------------------------------------------------------
	const Ogre::ColourValue& ActorLight::getSpecular() {
		return mLight->getSpecularColour();
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDiffuse(const Ogre::ColourValue& _col) {
		return mLight->setDiffuseColour(_col);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setSpecular(const Ogre::ColourValue& _col) {
		return mLight->setSpecularColour(_col);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::_updateRenderQueue(Ogre::RenderQueue *queue) {
		static Ogre::AxisAlignedBox ab;
		ab.setNull();
		Ogre::Vector3 c = mLight->getBoundingBox().getCenter();
		ab.setExtents(c.x - mLight->getAttenuationRange(), c.y - mLight->getAttenuationRange(), c.z - mLight->getAttenuationRange(), c.x + mLight->getAttenuationRange(), c.y + mLight->getAttenuationRange(), c.z + mLight->getAttenuationRange());
		mRangeBox->setupBoundingBox(ab);
		queue->addRenderable(mRangeBox);
	}

	//-------------------------------------------------------------------------------------
	const Ogre::AxisAlignedBox& ActorLight::getBoundingBox(void) const{
		// Null, lights are not visible
		static Ogre::AxisAlignedBox box(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
		return box;
	}
}