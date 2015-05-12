#pragma once
#include "Obstacle.h"
#include "RecastConvexHull.h"

namespace vega
{
	class ConvexShapeObstacle : public Obstacle
	{
	public:
		/**
		  * Construct a convex shape obstacle on the specified position.
		  * To make this work with detour tileCache a simplified convex hull is created
		  * from the obstacle entity's geometry. The hull is offset with the specified
		  * amount from the mesh (offset should generally be agent radius).
		  * DetourTileCache parameter specifies the tilecache on which the obstacle will
		  * be added.
		  **/
		ConvexShapeObstacle(Ogre::Vector3 position, Ogre::Real offset, OgreDetourTileCache *detourTileCache);
		virtual ~ConvexShapeObstacle(); // Important that the destructor is virtual!

		/**
		  * @see{Obstacle::update()}
		  **/
		virtual void update(long time);

		/**
		  * @see{Obstacle::getEntity()}
		  **/
		virtual Ogre::Entity* getEntity(void);

		/**
		  * @see{Obstacle::getPosition()}
		  **/
		virtual Ogre::Vector3 getPosition(void);

		/**
		  * @see{Obstacle::getOrientation()}
		  **/
		virtual Ogre::Quaternion getOrientation(void);

		/**
		  * Update the orientation (mainly intended for rotation and position) of the obstacle
		  * to the specified orientation.
		  * Replaces the old rotation completely (this is not cumulative).
		  **/
		virtual void updateOrientation(Ogre::Quaternion orientation);

		/**
		  * @see{Obstacle::updatePosition(Ogre::Vector3)}
		  **/
		virtual void updatePosition(Ogre::Vector3 position);

	protected:
		/**
		  * Current position of this obstacle.
		  **/
		Ogre::Vector3 mPosition;

		/**
		  * Current orientation of this obstacle.
		  **/
		Ogre::Quaternion mOrientation;

		/**
		  * The entity representing this obstacle in the scene.
		  **/
		Ogre::Entity *mEnt;

		/**
		  * The scene node in which the obstacle entity is located.
		  **/
		Ogre::SceneNode *mNode;

		/**
		  * Name of this obstacle.
		  **/
		Ogre::String mName;

		/**
		  * 2D Convex hull with height calculated for this obstacle (for the current rotation).
		  **/
		ConvexVolume *mConvexHull;

		/**
		  * Inputgeom object containing the original geometry of this obstacle (used to generate convex hull from).
		  **/
		InputGeom *mInputGeom;

		/**
		  * Amount that the convex hull is offset from the inputGeom. Normally detour requires the hulls to be offset
		  * with at least the radius of the agents.
		  **/
		Ogre::Real mOffset;

		/**
		  * Visual debug representation of the calculated convex hull, in the form of a line drawing.
		  **/
		Ogre::ManualObject *mConvexHullDebug;
	};
}