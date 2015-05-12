#pragma warning(disable:4355)

#include "SkyX.h"
#include "EnginePrivate.h"

namespace vega
{
	SkyX::SkyX(Ogre::SceneManager* sm, Ogre::Camera* _cam,
		Ogre::Root *_mRoot, Ogre::RenderWindow* _mWindow, Controller* c)
		: Ogre::RenderTargetListener()
		, mRoot(_mRoot)
		, mWindow(_mWindow)
		, mSceneManager(sm)
		, mController(c)
		, mCamera(_cam)
		, mMeshManager(new MeshManager(this))
		, mAtmosphereManager(new AtmosphereManager(this))
		, mGPUManager(new GPUManager(this))
		, mMoonManager(new MoonManager(this))
		, mCloudsManager(new CloudsManager(this))
		, mRenderQueueGroups(RenderQueueGroups(Ogre::RENDER_QUEUE_SKIES_EARLY, Ogre::RENDER_QUEUE_3, Ogre::RENDER_QUEUE_4, Ogre::RENDER_QUEUE_2))
		, mCreated(false)
		, mLastCameraPosition(Ogre::Vector3(0,0,0))
		, mLastCameraFarClipDistance(-1)
		, mInfiniteCameraFarClipDistance(100000)
		, mVisible(true)
		, mLightingMode(LM_LDR)
		, mStarfield(true)
		, mTimeMultiplier(0.1f)
		, mTimeOffset(0.0f)
		, mSun(nullptr)
	{
		// Need to be instanced here, when SkyX::mSceneManager is valid
		mVCloudsManager = new VCloudsManager(this);
	}

	SkyX::~SkyX()
	{
		remove();

		delete mMeshManager;
		delete mAtmosphereManager;
		delete mGPUManager;
		delete mMoonManager;
		delete mCloudsManager;
		delete mVCloudsManager;

		if (mController->getDeleteBySkyX())
			delete mController;
		
		if (mSun)
			delete mSun;
	}

	void SkyX::create()
	{
		if (mCreated)
			return;

		mGPUManager->_notifySkydomeMaterialChanged();

		mMeshManager->create();
		mMeshManager->setMaterialName(mGPUManager->getSkydomeMaterialName());

		mAtmosphereManager->_update(mAtmosphereManager->getOptions(), true);

		if (mStarfield)
		{
			mGPUManager->setGpuProgramParameter(GPUManager::GPUP_FRAGMENT, "uTime", mTimeOffset*0.5f, false);
		}

		mGPUManager->setGpuProgramParameter(GPUManager::GPUP_VERTEX, "uLightDir", mController->getSunDirection());
		mGPUManager->setGpuProgramParameter(GPUManager::GPUP_FRAGMENT, "uLightDir", mController->getSunDirection());

		mMoonManager->create();

		setVisible(mVisible);

		mLastCameraPosition = Ogre::Vector3(0,0,0);
		mLastCameraFarClipDistance = -1;
		//NickGalko
		mSun = new ActorLight("Sun", ActorLight::DIRECTIONAL);
		mSun->setDiffuse(255, 255, 255);
		mSun->setDirection(-(mController->getSunDirection()));
	
		mCreated = true;	

		mWindow->addListener(this);
	}

	void SkyX::remove()
	{
		if (!mCreated)
			return;

		mWindow->removeListener(this);

		mCloudsManager->removeAll();
		mMeshManager->remove();
		mMoonManager->remove();
		mVCloudsManager->remove();

		mCamera = 0;
		mCreated = false;
	}

	void SkyX::update(const Ogre::Real& timeSinceLastFrame)
	{
		if (!mCreated)
			return;

		if (mTimeMultiplier != 0)
		{
			float timemultiplied = timeSinceLastFrame * mTimeMultiplier;

			mTimeOffset += timemultiplied;

			mController->update(timemultiplied);

			if (mStarfield)
				mGPUManager->setGpuProgramParameter(GPUManager::GPUP_FRAGMENT, "uTime", mTimeOffset*0.5f, false);
			
		}

		mGPUManager->setGpuProgramParameter(GPUManager::GPUP_VERTEX, "uLightDir", mController->getSunDirection());
		mGPUManager->setGpuProgramParameter(GPUManager::GPUP_FRAGMENT, "uLightDir", mController->getSunDirection());

		mMoonManager->updateMoonPhase(mController->getMoonPhase());
		mCloudsManager->update();
		mVCloudsManager->update(timeSinceLastFrame);
//NickGalko
		updateEnvironmentLighting();
	}
	/** Update environment lighting
	 */
	void SkyX::updateEnvironmentLighting()
	{
		Ogre::Vector3 lightDir = mController->getSunDirection();
		// Calculate current color gradients point
		float point = (-lightDir.y + 1.0f) / 2.0f;
		
		mSun->setDirection(-lightDir.normalisedCopy());
		// Sun and ambient colours
		Ogre::ColourValue sunCol = VectorToColour(
			getVCloudsManager()->getSunGradient().getColor(point));

		Ogre::ColourValue ambCol = VectorToColour(
			getVCloudsManager()->getAmbientGradient().getColor(point));

		// Minimum ambient light
		Ogre::ColourValue minAmbientLight(0.1f, 0.1f, 0.2f);

		// Sun multipliers
		Ogre::ColourValue sunSpecularMult(0.9f, 0.9f, 0.9f);
		Ogre::ColourValue sunDiffuseMult(0.8f, 0.8f, 0.7f);
		Ogre::ColourValue sunAmbientMult(0.5f, 0.5f, 0.5f);


		mSun->setSpecular(ambCol * sunSpecularMult);
		mSun->setDiffuse(sunCol * sunDiffuseMult);
	}
	void SkyX::notifyCameraRender(Ogre::Camera* c)
	{
		if (!mCreated)
			return;

		mCamera = c;
		
		if (mLastCameraPosition != c->getDerivedPosition())
		{
			mMeshManager->getSceneNode()->setPosition(mCamera->getDerivedPosition());

			mLastCameraPosition = mCamera->getDerivedPosition();
		}

		if (mLastCameraFarClipDistance != c->getFarClipDistance())
		{
			mMeshManager->updateGeometry(c);

			mLastCameraFarClipDistance = mCamera->getFarClipDistance();
		}

		mMoonManager->updateGeometry(c);

		mVCloudsManager->notifyCameraRender(c);
	}

	void SkyX::setVisible(const bool& visible)
	{
		mVisible = visible;

		if (!mCreated)
			return;

		mMeshManager->getSceneNode()->setVisible(mVisible);
		mMoonManager->getMoonSceneNode()->setVisible(mVisible);

		if (mVCloudsManager->isCreated())
			mVCloudsManager->getVClouds()->setVisible(mVisible);
	}

	void SkyX::setRenderQueueGroups(const RenderQueueGroups& rqg)
	{
		mRenderQueueGroups = rqg;

		mVCloudsManager->getVClouds()->setRenderQueueGroups(
			VClouds::VClouds::RenderQueueGroups(mRenderQueueGroups.vclouds, mRenderQueueGroups.vcloudsLightningsUnder, mRenderQueueGroups.vcloudsLightningsOver));

		if (!mCreated)
			return;

		mMeshManager->getEntity()->setRenderQueueGroup(mRenderQueueGroups.skydome);
		mMoonManager->getMoonBillboard()->setRenderQueueGroup(mRenderQueueGroups.skydome+1);
	}

	void SkyX::setLightingMode(const LightingMode& lm)
	{
		mLightingMode = lm;

		if (!mCreated)
			return;

		mGPUManager->_notifySkydomeMaterialChanged();

		// Update skydome material
		mMeshManager->setMaterialName(mGPUManager->getSkydomeMaterialName());
		// Update layered clouds material
		mCloudsManager->registerAll();
		// Update ground passes materials and update textures gamma correction
		mGPUManager->_updateFP();

		// Update parameters
		mAtmosphereManager->_update(mAtmosphereManager->getOptions(), true);
	}

	void SkyX::setStarfieldEnabled(const bool& Enabled)
	{
		mStarfield = Enabled;

		if (!mCreated)
			return;

		mGPUManager->_notifySkydomeMaterialChanged();

		// Update skydome material
		mMeshManager->setMaterialName(mGPUManager->getSkydomeMaterialName());

		// Update parameters
		mAtmosphereManager->_update(mAtmosphereManager->getOptions(), true);

		if (mStarfield)
			mGPUManager->setGpuProgramParameter(GPUManager::GPUP_FRAGMENT, "uTime", mTimeOffset*0.5f, false);

		mGPUManager->setGpuProgramParameter(GPUManager::GPUP_VERTEX, "uLightDir", mController->getSunDirection());
		mGPUManager->setGpuProgramParameter(GPUManager::GPUP_FRAGMENT, "uLightDir", mController->getSunDirection());
	}

	void SkyX::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		notifyCameraRender(evt.source->getCamera());
	}
}