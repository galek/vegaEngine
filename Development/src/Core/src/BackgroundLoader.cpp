#include "CorePrivate.h"
#include "BackgroundLoader.h"

namespace vega
{
	ResourceGroupBackgroundLoader::ResourceGroupBackgroundLoader(SubSystemsManager* _systems, DWORD loadingIntervalMs)
		: mLoadingIntervalMs(loadingIntervalMs)
		, systems(_systems)
	{
		Ogre::WorkQueue* wq = systems->mGRoot->getWorkQueue();
		mWorkQueueChannel = wq->getChannel("core/ResourceGroupBackgroundLoader");
		wq->addRequestHandler(mWorkQueueChannel, this);
		wq->addResponseHandler(mWorkQueueChannel, this);

		systems->mGRoot->addFrameListener(this);
	}


	ResourceGroupBackgroundLoader::~ResourceGroupBackgroundLoader()
	{
		systems->mGRoot->removeFrameListener(this);

		Ogre::WorkQueue* wq = systems->mGRoot->getWorkQueue();
		wq->abortRequestsByChannel(mWorkQueueChannel);
		wq->removeRequestHandler(mWorkQueueChannel, this);
		wq->removeResponseHandler(mWorkQueueChannel, this);
	}


	void ResourceGroupBackgroundLoader::BackgroundLoadResourceGroup(const Ogre::String& resGroupName, const Ogre::String& location, Listener* listener)
	{
		Debug(__FUNCTION__);
		BUG("Crash using iterator")
#if 0//Crash using iterator
		for (auto it = mResGroupRequestQueue.begin();
			it != mResGroupRequestQueue.end(); ++it)
		{
			if (it->name == resGroupName &&
				it->type == UNLOAD_GROUP && it->status == INITIALISE)
			{
				mResGroupRequestQueue.erase(it);
				break;
			}
		}

		auto requestQueueItor = mResGroupRequestQueue.begin();
		for (; requestQueueItor != mResGroupRequestQueue.end(); ++requestQueueItor)
		{
			if (requestQueueItor->name == resGroupName &&
				requestQueueItor->type == LOAD_GROUP &&
				requestQueueItor->listener != 0)
				break;
		}

		if (requestQueueItor == mResGroupRequestQueue.end())
		{
			ResGroupRequest newResGroupReq = { LOAD_GROUP, INITIALISE, resGroupName, location, listener };
			mResGroupRequestQueue.push_back(newResGroupReq);
		}
#endif
	}


	void ResourceGroupBackgroundLoader::BackgroundUnLoadResourceGroup(const Ogre::String& resGroupName, Listener* listener)
	{
		BUG("Crash using iterator")
#if 0//Crash using iterator
		for (ResGroupRequestQueue::iterator requestQueueItor = mResGroupRequestQueue.begin();
			requestQueueItor != mResGroupRequestQueue.end(); ++requestQueueItor)
		{
			if (requestQueueItor->name == resGroupName &&
				requestQueueItor->type == LOAD_GROUP)
			{
				if (requestQueueItor->listener == listener)
					requestQueueItor->listener = 0;

				if (requestQueueItor->status == INITIALISE)
				{
					mResGroupRequestQueue.erase(requestQueueItor);
					break;
				}
			}
		}

		ResGroupRequestQueue::iterator requestQueueItor = mResGroupRequestQueue.begin();
		for (; requestQueueItor != mResGroupRequestQueue.end(); ++requestQueueItor)
		{
			if (requestQueueItor->name == resGroupName &&
				requestQueueItor->type == UNLOAD_GROUP)
				break;
		}

		if (requestQueueItor == mResGroupRequestQueue.end())
		{
			ResGroupRequest newResGroupReq = { UNLOAD_GROUP, INITIALISE, resGroupName };
			mResGroupRequestQueue.push_back(newResGroupReq);
		}
#endif
	}


	bool ResourceGroupBackgroundLoader::Loading()
	{
		if (!mResGroupRequestQueue.empty())
		{
			ResGroupRequest& resGroupReq = mResGroupRequestQueue.front();
			if (resGroupReq.status == INITIALISE)
			{
				if (resGroupReq.type == LOAD_GROUP)
				{
					Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
					if (!resGroupMgr.resourceGroupExists(resGroupReq.name))
					{
						resGroupMgr.createResourceGroup(resGroupReq.name, false);
						resGroupMgr.addResourceLocation(resGroupReq.location, "FileSystem", resGroupReq.name);
					}

					if (resGroupReq.listener)
						resGroupReq.listener->resGroupCreated(resGroupReq.name);

					Request req;
					req.type = resGroupReq.type;
					req.resGroupName = resGroupReq.name;
					Ogre::Root::getSingleton().getWorkQueue()->addRequest(mWorkQueueChannel, resGroupReq.type, Ogre::Any(req));

					resGroupReq.status = PREPARING;
				}
				else if (resGroupReq.type == UNLOAD_GROUP)
				{
					Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
					if (resGroupMgr.resourceGroupExists(resGroupReq.name))
						resGroupReq.meshes = Ogre::ResourceGroupManager::getSingleton().findResourceNames(resGroupReq.name, "*.mesh");
					getResourcesFromResourceGroup(false);
					resGroupReq.status = PREPARED;
				}
			}
			else if (resGroupReq.status == PREPARED)
			{
				DWORD beginTime = GetTickCount();
				DWORD elapseTime = 0;
				while (elapseTime < mLoadingIntervalMs)
				{
					if (!resGroupReq.resourceQueue.empty())
					{
						auto res = resGroupReq.resourceQueue.front();
						if (resGroupReq.type == LOAD_GROUP)
						{
							res->load(true);
						}
						else if (resGroupReq.type == UNLOAD_GROUP)
						{
							res->unload();
						}

						if (res->getCreator() == Ogre::MeshManager::getSingletonPtr() &&
							resGroupReq.listener)
						{
							if (resGroupReq.type == LOAD_GROUP)
							{
								res->_fireLoadingComplete(true);
								resGroupReq.listener->meshLoaded(res->getName());
							}
							else if (resGroupReq.type == UNLOAD_GROUP)
							{
								res->_fireUnloadingComplete();
								resGroupReq.listener->meshUnLoaded(res->getName());
							}
						}

						resGroupReq.resourceQueue.pop_front();
					}
					else
						break;

					elapseTime = GetTickCount() - beginTime;
				}

				if (resGroupReq.resourceQueue.empty())
					getResourcesFromResourceGroup(resGroupReq.type == LOAD_GROUP);

				if (resGroupReq.resourceQueue.empty())
					resGroupReq.status = COMPLETED;
			}
			else if (resGroupReq.status == COMPLETED)
			{
				Ogre::String resGroupName = resGroupReq.name;
				mResGroupRequestQueue.pop_front();

				Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
				switch (resGroupReq.type)
				{
				case LOAD_GROUP:
					resGroupMgr.loadResourceGroup(resGroupName, true, false);
					if (resGroupReq.listener)
						resGroupReq.listener->resGroupLoaded(resGroupName);
					break;

				case UNLOAD_GROUP:
					if (resGroupMgr.resourceGroupExists(resGroupName))
						resGroupMgr.destroyResourceGroup(resGroupName);
					break;

				default:
					break;
				}
			}
		}

		return true;
	}


	Ogre::WorkQueue::Response* ResourceGroupBackgroundLoader::handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ)
	{
		Request resreq = Ogre::any_cast<Request>(req->getData());

		if (req->getAborted())
		{
			return OGRE_NEW Ogre::WorkQueue::Response(req, true, Ogre::Any());
		}

		try
		{
			Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
			switch (resreq.type)
			{
			case LOAD_GROUP:
			{
				resGroupMgr.initialiseResourceGroup(resreq.resGroupName);
				resGroupMgr.prepareResourceGroup(resreq.resGroupName, true, false);
			}
			break;
			}
		}
		catch (Ogre::Exception& e)
		{
			const char* error = e.what();
			ErrorF(error);
		}

		return OGRE_NEW Ogre::WorkQueue::Response(req, true, Ogre::Any(resreq));
	}


	void ResourceGroupBackgroundLoader::handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ)
	{
		if (res->getRequest()->getAborted())
			return;

		if (res->succeeded())
		{
			Request resreq = Ogre::any_cast<Request>(res->getData());
			switch (resreq.type)
			{
			case LOAD_GROUP:
			{
				ResGroupRequest& resGroupReq = mResGroupRequestQueue.front();
				resGroupReq.status = PREPARED;
				resGroupReq.meshes = Ogre::ResourceGroupManager::getSingleton().findResourceNames(resreq.resGroupName, "*.mesh");
				getResourcesFromResourceGroup(true);
			}
			break;
			}
		}
	}


	void ResourceGroupBackgroundLoader::getResourcesFromResourceGroup(bool load)
	{
		auto resGroupReq = mResGroupRequestQueue.front();
		if (!resGroupReq.meshes.isNull() &&
			!resGroupReq.meshes->empty())
		{
			const Ogre::String& meshName = resGroupReq.meshes->back();
			Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().getByName(meshName);
			if (load)
				meshPtr->load(true);

			unsigned short numSubMeshes = meshPtr->getNumSubMeshes();
			for (unsigned short sm = 0; sm < numSubMeshes; ++sm)
			{
				Ogre::SubMesh* subMesh = meshPtr->getSubMesh(sm);
				if (subMesh->isMatInitialised())
				{
					Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(subMesh->getMaterialName());
					if (!materialPtr.isNull())
					{
						unsigned short numSupportedTech = materialPtr->getNumSupportedTechniques();
						for (unsigned short st = 0; st < numSupportedTech; ++st)
						{
							Ogre::Technique* tech = materialPtr->getSupportedTechnique(st);
							unsigned short numPass = tech->getNumPasses();
							for (unsigned short p = 0; p < numPass; ++p)
							{
								Ogre::Pass* pass = tech->getPass(p);
								unsigned short numTexUnitState = pass->getNumTextureUnitStates();
								for (unsigned short uts = 0; uts < numTexUnitState; ++uts)
								{
									Ogre::TextureUnitState* texUnitState = pass->getTextureUnitState(uts);
									unsigned int numFrames = texUnitState->getNumFrames();
									for (unsigned int f = 0; f < numFrames; ++f)
									{
										const Ogre::String& frame = texUnitState->getFrameTextureName(f);
										Ogre::TexturePtr texturePtr = Ogre::TextureManager::getSingleton().getByName(frame);
										if (!texturePtr.isNull())
											resGroupReq.resourceQueue.push_back(texturePtr);
									}
								}
							}
						}

						resGroupReq.resourceQueue.push_back(materialPtr);
					}
				}
			}

			if (!load)
				meshPtr->unload();

			resGroupReq.resourceQueue.push_back(meshPtr);
			resGroupReq.meshes->pop_back();
		}
	}
}