/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
// Last Update:18.07.15
// BugFix:изменил функцию установки типа света,добавил функцию добавления дальности прорисовки
#include "EnginePrivate.h"
#include "ActorLight.h"
#include "SceneManager.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	ActorLight::ActorLight(std::string _mName, LightTypes _type, Ogre::Vector3 _pos, Ogre::Vector3 _rot)
	{
		mName = "ActorLight_" + _mName;
		m_actDesc = ActorDescription::AD_LIGHT;
		
		mLight = GetEngine()->mGSceneMgr->createLight(mName);
		mNode = GetEngine()->mGSceneMgr->getRootSceneNode()->createChildSceneNode(mName);

		attach(this);

		//Ставим изначальные параметры
		setType(_type);
		setPosition(_pos);
		setDirection(_rot.normalisedCopy());
		setCastShadows(true);
	}

	//-------------------------------------------------------------------------------------
	ActorLight::ActorLight(const Ogre::Light&_mLight)
		:ActorLight(_mLight.getName(), (ActorLight::LightTypes)_mLight.getType(), _mLight.getPosition(), _mLight.getDirection()) {
		mLight = (Ogre::Light*)&_mLight;
	}

	//-------------------------------------------------------------------------------------
	ActorLight::ActorLight(Ogre::Light*_mLight)
		: ActorLight(_mLight->getName(), (ActorLight::LightTypes)_mLight->getType(), _mLight->getPosition(), _mLight->getDirection())
	{
		mLight = _mLight;
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
		mNode->detachAllObjects();
		mNode->getCreator()->destroyLight(mLight);
		mNode->removeAndDestroyAllChildren();
		mNode->getCreator()->getRootSceneNode()->removeAndDestroyChild(mNode->getName());

		mLight = nullptr;

		mNode = nullptr;
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setShadowFarClipDistance(float _d) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mLight->setShadowFarClipDistance(_d);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setShadowFarDistance(float _d) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mLight->setShadowFarDistance(_d);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::attach(Actor *_mParent) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		_mParent->getNode()->attachObject(mLight);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setType(LightTypes _type)
	{
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		if (mLightType != _type)
		{
			mLightType = (_type);
			mLight->setType(Ogre::Light::LightTypes(_type));
		}
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setVisible(bool _status) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mLight->setVisible(_status);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setCastShadows(bool _status) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mLight->setCastShadows(_status);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setPosition(const Ogre::Vector3& _pos) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mLight->setPosition(_pos);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDirection(const Ogre::Vector3& _quat) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mLight->setDirection(_quat);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDiffuse(float _r, float _g, float _b) {
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mLight->setDiffuseColour(_r, _g, _b);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setSpecular(float _r, float _g, float _b) {
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			return;
		}
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
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			return;
		}
		mLight->setAttenuation(_range, _constant, _linear, _quadratic);
	}

	//-------------------------------------------------------------------------------------
	const Ogre::Vector3& ActorLight::getDirection() {
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			static Ogre::Vector3 error;
			return error;
		}
		return mLight->getDirection();
	}

	//-------------------------------------------------------------------------------------
	const Ogre::ColourValue& ActorLight::getDiffuse() {
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			static Ogre::ColourValue error(1, 0, 0);
			return error;
		}
		return mLight->getDiffuseColour();
	}

	//-------------------------------------------------------------------------------------
	const Ogre::ColourValue& ActorLight::getSpecular() {
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			static Ogre::ColourValue error(1, 0, 0);
			return error;
		}
		return mLight->getSpecularColour();
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setDiffuse(const Ogre::ColourValue& _col) {
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			return;
		}
		mLight->setDiffuseColour(_col);
	}

	//-------------------------------------------------------------------------------------
	void ActorLight::setSpecular(const Ogre::ColourValue& _col) {
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			return;
		}
		mLight->setSpecularColour(_col);
	}

	//-------------------------------------------------------------------------------------
	const Ogre::AxisAlignedBox& ActorLight::getBoundingBox(void) const {
		// Null, lights are not visible
		static Ogre::AxisAlignedBox box(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
		return box;
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetSpotOuter() const
	{
		if (!isValid(__FUNCTION__))
		{
			return 0;
		}
		return mLight->getSpotlightOuterAngle().valueRadians();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetSpotInner() const
	{
		if (!isValid(__FUNCTION__))
		{
			return 0;
		}
		return mLight->getSpotlightInnerAngle().valueRadians();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetSpotlightFalloff() const
	{
		if (!isValid(__FUNCTION__))
		{
			return 0;
		}
		return mLight->getSpotlightFalloff();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationRange(void) const
	{
		if (!isValid(__FUNCTION__))
		{
			return 0;
		}
		return mLight->getAttenuationRange();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationConstant(void) const
	{
		if (!isValid(__FUNCTION__))
		{
			return 0;
		}
		return mLight->getAttenuationConstant();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationLinear(void) const
	{
		if (!isValid(__FUNCTION__))
		{
			return 0;
		}
		return mLight->getAttenuationLinear();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetAttenuationQuadric(void) const
	{
		if (!isValid(__FUNCTION__))
		{
			return 0;
		}
		return mLight->getAttenuationQuadric();
	}

	//-------------------------------------------------------------------------------------
	float ActorLight::GetPowerScale(void) const
	{
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", __FUNCTION__);
			return 0;
		}
		return mLight->getPowerScale();
	}

	//-------------------------------------------------------------------------------------
	bool ActorLight::isValid(const char* _funct) const
	{
		if (!mLight)
		{
			Warning("mLight is null,in function:%s", _funct);
			return false;
		}
		return true;
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

}