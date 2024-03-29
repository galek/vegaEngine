/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com



Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/


#ifndef _OGREBULLETCOLLISIONS_RagDoll_H
#define _OGREBULLETCOLLISIONS_RagDoll_H


#include "OgreBulletDynamicsPreRequisites.h"


namespace vega
{
	// -------------------------------------------------------------------------
	// basic CollisionWorld
	class RagDoll 
	{
	public:
        RagDoll(btDynamicsWorld *ownerWorld,
                const btVector3 &positionOffset);

        virtual	~RagDoll();

		// Make ragdoll  active.
		void ragdollEnable(bool doEnable);
		// is ragdoll active.
		bool isRagdollEnabled();

		// Make constraint between body part active.
		void constraintEnable(bool value);
		// are constraint between body part active ?
		bool isConstraintEnabled();

		// Make it collidable or not.
		void collisionEnable(bool value);
		// if shapes can collide
		bool isCollisionEnabled();

		// Make it fixed in a pose or not.
		void rigidityEnable(bool value);
		// is it fixed on a pose?
		bool isRigidityEnable();

	protected:
		btDynamicsWorld* m_ownerWorld;

        std::vector<btCollisionShape *>  m_shapes;
        std::vector<btRigidBody *>		 m_bodies;
        std::vector<btTypedConstraint *> m_joints;

        btRigidBody *localCreateRigidBody(btScalar mass,
                                          const btTransform &startTransform,
                                          btCollisionShape *shape);
	};
}
#endif //_OGREBULLETCOLLISIONS_CollisionWorld_H

