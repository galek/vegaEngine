#pragma once

#include <OgrePrerequisites.h>
#include <OgreWorkQueue.h>
#include <OgreStringVector.h>
#include <OgreSingleton.h>
#include <OgreFrameListener.h>

#ifndef CORE_API
#include "Core.h"
#endif

namespace vega
{
	struct SubSystemsManager;

	class CORE_API ResourceGroupBackgroundLoader : public Ogre::WorkQueue::RequestHandler, public Ogre::WorkQueue::ResponseHandler, public Ogre::FrameListener
	{
	public:

		struct Listener
		{
			virtual void resGroupCreated(const Ogre::String& resGroupName) {}
			virtual void resGroupLoaded(const Ogre::String& resGroupName) {}
			virtual void resGroupUnLoaded(const Ogre::String& resGroupName) {}
			virtual void meshLoaded(const Ogre::String& meshName) {}
			virtual void meshUnLoaded(const Ogre::String& meshName) {}
			virtual ~Listener() {}
		};

		ResourceGroupBackgroundLoader(SubSystemsManager*, DWORD loadingIntervalMs);
		~ResourceGroupBackgroundLoader();

		void BackgroundLoadResourceGroup(const Ogre::String& resGroupName, const Ogre::String& location, Listener* listener = 0);
		void BackgroundUnLoadResourceGroup(const Ogre::String& resGroupName, Listener* listener = 0);


		bool Loading();

	private:
		ResourceGroupBackgroundLoader() {}

		bool canHandleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ) { return true; }
		Ogre::WorkQueue::Response* handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
		bool canHandleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ) { return true; }
		void handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);

		void getResourcesFromResourceGroup(bool load);
	private:
		enum RequestType
		{
			LOAD_GROUP,
			UNLOAD_GROUP,
		};

		enum ResGroupStatus
		{
			INITIALISE,
			PREPARING,
			PREPARED,
			COMPLETED
		};

		struct ResGroupRequest
		{
			RequestType					type;
			ResGroupStatus				status;
			Ogre::String				name;
			Ogre::String				location;
			Listener*					listener;
			Ogre::StringVectorPtr		meshes;

			typedef std::deque<Ogre::ResourcePtr> ResourceQueue;
			ResourceQueue resourceQueue;
		};
		typedef std::deque<ResGroupRequest> ResGroupRequestQueue;


		DWORD							mLoadingIntervalMs;
		ResGroupRequestQueue			mResGroupRequestQueue;

		Ogre::uint16					mWorkQueueChannel;

		struct Request
		{
			RequestType				type;
			Ogre::String			resGroupName;
			Ogre::String			resName;

			friend std::ostream& operator << (std::ostream& o, const Request& r)
			{
				(void)r; return o;
			}
		};
	private:
		SubSystemsManager* systems;
	};
}