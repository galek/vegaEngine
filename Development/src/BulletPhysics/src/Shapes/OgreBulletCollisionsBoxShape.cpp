#include "OgreBulletCollisions.h"

#include "Utils/OgreBulletConverter.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

using namespace Ogre;


namespace vega
{
    // -------------------------------------------------------------------------
    BoxCollisionShape::BoxCollisionShape(const Vector3 &bodyBounds)
        : CollisionShape()
    {
        mShape = new btBoxShape(OgreBtConverter::to(bodyBounds));
    }
    // -------------------------------------------------------------------------
    BoxCollisionShape::~BoxCollisionShape()
    {
    }
}

