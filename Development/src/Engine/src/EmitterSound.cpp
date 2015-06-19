/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2015 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "EmitterSound.h"


namespace vega
{
	EmitterSound::EmitterSound(const std::string& _name, const std::string& fileName, bool loop, bool stream)
	{
		TODO("Возможный баг,непонятно,что с именем-это или имя ресурса,или имя актера");
		mName = "EmitterSound_" + _name;//for Actor Base
		m_actDesc = ActorDescription::AD_EMITTER_SOUND;

		sound = GetEngine()->GetAudio()->createSound(_name, fileName, loop, stream);
	}

	EmitterSound::~EmitterSound()
	{
		SAFE_DELETE(sound);
	}

	bool EmitterSound::play() { return sound->play(); }

	bool EmitterSound::isPlaying() const { return sound->isPlaying(); }

	bool EmitterSound::pause() { return sound->pause(); }

	bool EmitterSound::isPaused() const { return sound->isPaused(); }

	bool EmitterSound::stop() { return sound->stop(); }

	bool EmitterSound::isStopped() const { return sound->isStopped(); }

	bool EmitterSound::isInitial() const { return sound->isInitial(); }

	bool EmitterSound::isFading() const { return sound->isFading(); }

	bool EmitterSound::fadeIn(float fadeTime) { return sound->fadeIn(fadeTime); }

	bool EmitterSound::fadeOut(float fadeTime) { return sound->fadeOut(fadeTime); }

	bool EmitterSound::cancelFade() { return sound->cancelFade(); }

	void EmitterSound::setPitch(float pitch)  { sound->setPitch(pitch); }

	float EmitterSound::getPitch() const { return sound->getPitch(); }

	void EmitterSound::setGain(float gain) { sound->setGain(gain); }

	float EmitterSound::getGain() const { return sound->getGain(); }

	void EmitterSound::setMaxGain(float maxGain)  { sound->setGain(maxGain); }

	float EmitterSound::getMaxGain() const { return sound->getMaxGain(); }

	void EmitterSound::setMinGain(float minGain) { sound->setMinGain(minGain); }

	float EmitterSound::getMinGain() const { return sound->getMinGain(); }

	float EmitterSound::getGainScale() const { return sound->getGainScale(); }

	void EmitterSound::setGainScale(float gainScale){ sound->setGainScale(gainScale); }

	void EmitterSound::setGainValues(float maxGain, float minGain,
		float gain, float gainScale) {
		sound->setGainValues(maxGain, minGain, gain, gainScale);
	}

	void EmitterSound::setMaxDistance(float maxDistance) {
		sound->setMaxDistance(maxDistance);
	}

	float EmitterSound::getMaxDistance() const { return sound->getMaxDistance(); }

	void EmitterSound::setRolloffFactor(float rolloffFactor)  {
		sound->setRolloffFactor(rolloffFactor);
	}

	float EmitterSound::getRolloffFactor() const { return sound->getRolloffFactor(); }

	void EmitterSound::setReferenceDistance(float refDistance)  {
		sound->setReferenceDistance(refDistance);
	}

	float EmitterSound::getReferenceDistance() const { return sound->getReferenceDistance(); }

	void EmitterSound::setDistanceValues(float maxDistance, float rolloffFactor, float refDistance) {
		sound->setDistanceValues(maxDistance, rolloffFactor, refDistance);
	}

	void EmitterSound::setVelocity(float x, float y, float z) {
		sound->setVelocity(x, y, z);
	}

	void EmitterSound::setVelocity(const Ogre::Vector3& vec) {
		sound->setVelocity(vec);
	}

	const Ogre::Vector3& EmitterSound::getVelocity() const { return sound->getVelocity(); }

	void EmitterSound::setRelativeToListener(bool relative) {
		sound->setRelativeToListener(relative);
	}

	void EmitterSound::setPosition(float x, float y, float z){
		sound->setPosition(x, y, z);
	}

	void EmitterSound::setPosition(const Ogre::Vector3& vec){
		sound->setPosition(vec);
	}

	const Ogre::Vector3& EmitterSound::getPosition() const { return sound->getPosition(); }

	void EmitterSound::setDirection(float x, float y, float z){
		sound->setDirection(x, y, z);
	}

	void EmitterSound::setDirection(const Ogre::Vector3& vec){
		sound->setDirection(vec);
	}

	const Ogre::Vector3& EmitterSound::getDirection() const { return sound->getDirection(); }

	void EmitterSound::setOuterConeGain(float outerConeGain) {
		sound->setOuterConeGain(outerConeGain);
	}

	float EmitterSound::getOuterConeGain() const { return sound->getOuterConeGain(); }

	void EmitterSound::setInnerConeAngle(float innerConeAngle) {
		sound->setInnerConeAngle(innerConeAngle);
	}

	float EmitterSound::getInnerConeAngle() const { return sound->getInnerConeAngle(); }

	void EmitterSound::setOuterConeAngle(float outerConeAngle) {
		sound->setOuterConeAngle(outerConeAngle);
	}

	float EmitterSound::getOuterConeAngle() { return sound->getOuterConeAngle(); }

	void EmitterSound::setLoop(bool loop) {
		sound->setLoop(loop);
	}

	bool EmitterSound::isStreaming() const { return sound->isStreaming(); }

	void EmitterSound::setPriority(int priority) { sound->setPriority((iSound::Priority)priority); }

	int EmitterSound::getPriority() const { return sound->getPriority(); }

	float EmitterSound::getSecondDuration() { return sound->getSecondDuration(); }

	void EmitterSound::setSecondOffset(float seconds)  {
		sound->setSecondOffset(seconds);
	}

	float EmitterSound::getSecondOffset() { return sound->getSecondOffset(); }

	const Ogre::Vector3& EmitterSound::getDerivedPosition() const { return sound->getDerivedPosition(); }

	const Ogre::Vector3& EmitterSound::getDerivedDirection() const { return sound->getDerivedDirection(); }

	const std::string& EmitterSound::getFileName() const { return sound->getFileName(); }
}