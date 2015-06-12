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
	ActorLight::ActorLight(std::string _mName, LightTypes _type, Ogre::Vector3 _pos, Ogre::Vector3 _rot)
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
			if (Ogre::MaterialManager::getSingleton().resourceExists(_mName)) {
				baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().getByName(_mName);
			}
			else {
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
	void ActorLight::setShadowFarClipDistance(float _d) {
		mLight->setShadowFarClipDistance(_d);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setShadowFarDistance(float _d) {
		mLight->setShadowFarDistance(_d);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::attach(Actor *_mParent) {
		_mParent->getNode()->attachObject(mLight);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setType(LightTypes _type)
	{
		if (mLightType != _type)
		{
			mLightType = (_type);
			mLight->setType(Ogre::Light::LightTypes(_type));
		}
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setVisible(bool _status) {
		mLight->setVisible(_status);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setCastShadows(bool _status) {
		mLight->setCastShadows(_status);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setPosition(const Ogre::Vector3& _pos) {
		mLight->setPosition(_pos);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDirection(const Ogre::Vector3& _quat) {
		mLight->setDirection(_quat);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDiffuse(float _r, float _g, float _b) {
		mLight->setDiffuseColour(_r, _g, _b);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setSpecular(float _r, float _g, float _b) {
		mLight->setSpecularColour(_r, _g, _b);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setScale(const Ogre::Vector3& _scale) {
		if (mNode)
			mNode->setScale(_scale);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setPowerScale(float _range, float _constant, float _linear, float _quadratic) {
		if (!mLight)
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
		mLight->setAttenuation(_range, _constant, _linear, _quadratic);
	}

	//-------------------------------------------------------------------------------------
	const Ogre::Vector3& ActorLight::getDirection() {
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			static Ogre::Vector3 error;
			return error;
		}
		return mLight->getDirection();
	}

	//-------------------------------------------------------------------------------------
	const Ogre::ColourValue& ActorLight::getDiffuse() {
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			static Ogre::ColourValue error(1, 0, 0);
			return error;
		}
		return mLight->getDiffuseColour();
	}

	//-------------------------------------------------------------------------------------
	const Ogre::ColourValue& ActorLight::getSpecular() {
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			static Ogre::ColourValue error(1, 0, 0);
			return error;
		}
		return mLight->getSpecularColour();
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDiffuse(const Ogre::ColourValue& _col) {
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return;
		}
		mLight->setDiffuseColour(_col);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setSpecular(const Ogre::ColourValue& _col) {
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return;
		}
		mLight->setSpecularColour(_col);
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
	const Ogre::AxisAlignedBox& ActorLight::getBoundingBox(void) const {
		// Null, lights are not visible
		static Ogre::AxisAlignedBox box(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
		return box;
	}

	//-------------------------------------------------------------------------------------
	ActorLight& ActorLight::operator=(Ogre::Light &_mLight)
	{
		mLight = &_mLight;
		return *this;
	}

	//-------------------------------------------------------------------------------------
	ActorLight& ActorLight::operator=(Ogre::Light *_mLight)
	{
		mLight = _mLight;
		return *this;
	}

	//-------------------------------------------------------------------------------------
	ActorLight::ActorLight(const Ogre::Light&_mLight)
		:ActorLight(_mLight.getName(), (ActorLight::LightTypes)_mLight.getType(), _mLight.getPosition(), _mLight.getDirection()) {
		mLight = (Ogre::Light*)&_mLight;
	}

	//-------------------------------------------------------------------------------------
	ActorLight::ActorLight(Ogre::Light*_mLight)
		:ActorLight(_mLight->getName(), (ActorLight::LightTypes)_mLight->getType(), _mLight->getPosition(), _mLight->getDirection())
	{
		mLight = _mLight;
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetSpotOuter() 
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getSpotlightOuterAngle().valueRadians();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetSpotInner()
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getSpotlightInnerAngle().valueRadians();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetSpotlightFalloff()
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getSpotlightFalloff();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationRange(void) const
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getAttenuationRange();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationConstant(void) const
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getAttenuationConstant();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationLinear(void) const
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getAttenuationLinear();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationQuadric(void) const
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getAttenuationQuadric();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetPowerScale(void) const
	{
		if (!mLight)
		{
			std::exception((std::string("mLight is null,in function:") + __FUNCTION__).c_str());
			return 0;
		}
		return mLight->getPowerScale();
	}
	
	
}