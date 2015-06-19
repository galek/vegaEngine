/*
--------------------------------------------------------------------------------
This source file is part of SkyX.
Visit http://www.paradise-studios.net/products/skyx/

Copyright (C) 2009-2012 Xavier Verguín González <xavyiy@gmail.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
--------------------------------------------------------------------------------
*/

#ifndef _SkyX_MeshManager_H_
#define _SkyX_MeshManager_H_

#include "Prerequisites.h"

namespace vega
{
	class SkyX;

    class DllExport MeshManager 
	{
	public:
		/** Vertex struct
		 */
		struct VERTEX
		{
			      // Position
			float x, y, z,
			      // Normalized position
			      nx, ny, nz,
				  // Texture coords
			      u, v,
				  // Opacity
				  o;
		};

		/** Constructor
		    @param s Parent SkyX pointer
		 */
		MeshManager(SkyX *s);

		/** Destructor 
		 */
		~MeshManager();

		/** Create our water mesh, geometry, entity, etc...
            @remarks Call it after setMaterialName()
         */
        void create();

		/** Remove all resources
		 */
		void remove();

		/** Update geometry
		    @param cam Camera
		 */
		void updateGeometry(Ogre::Camera* cam);

		/** Get mesh
            @return Mesh
         */
        FORCEINLINE Ogre::Mesh* getMesh()
        {
            return mMesh;
        }

        /** Get sub mesh
            @return Sub mesh
         */
		FORCEINLINE Ogre::SubMesh* getSubMesh()
        {
            return mSubMesh;
        }

        /** Get entity
            @return Entity
         */
		FORCEINLINE Ogre::Entity* getEntity()
        {
            return mEntity;
        }

        /** Get material name
            @return Material name
         */
        FORCEINLINE const Ogre::String& getMaterialName() const
        {
            return mMaterialName;
        }

		/** Set mesh material
            @param MaterialName The material name
         */
        void setMaterialName(const Ogre::String &MaterialName);

		/** Get hardware vertex buffer reference
            @return Ogre::HardwareVertexBufferSharedPtr reference
         */
		FORCEINLINE Ogre::HardwareVertexBufferSharedPtr &getHardwareVertexBuffer()
        {
            return mVertexBuffer;
        }

		/** Get hardware index buffer reference
		    @return Ogre::HardwareIndexBufferSharedPtr reference
		 */
		FORCEINLINE Ogre::HardwareIndexBufferSharedPtr &getHardwareIndexBuffer()
		{
			return mIndexBuffer;
		}

		/** Get the Ogre::SceneNode pointer where Hydrax mesh is attached
		    @return Ogre::SceneNode*
		 */
		FORCEINLINE Ogre::SceneNode* getSceneNode()
		{
			return mSceneNode;
		}

		/** Is _createGeometry() called?
		    @return true if created() have been already called
		 */
		FORCEINLINE const bool& isCreated() const
		{
			return mCreated;
		}

		/** Set geometry parameters
		    @param Steps Sphere number of steps
			@param Circles Spehere number of circes
		 */
		void setGeometryParameters(const int& Steps, const int& Circles);

		/** Get number of steps
		    @return Number of steps
		 */
		FORCEINLINE const int& getSteps() const
		{
			return mSteps;
		}

		/** Get number of circles
		    @return Number of circles
		 */
		FORCEINLINE const int& getCircles() const
		{
			return mCircles;
		}

		/** Set under-horizon rendering params
		    @remarks In an ideal situation, you only must see the avobe horizon sky due to the fact that the terrain/water
			         must be 'infinite' and the under-horizont sky part is hide.
					 But, infinite terrain/water is not always implemented in games and 3d apps in general, so... in order to
					 get a good-looking sky, SkyX provides an approach to render realistic under-horizont sky.
			@param UnderHorizonCircles Number of circles of vega::MeshManager::mCircles reserved for the under-horizont geometry part,
				   0 means not under-horizon rendering
			@param UnderHorizonFading true/false to fade or not the under-horizon sky
			@param UnderHorizonFadingExponent Exponent of the fading, pow(vertex_angle, exp), 1=linear
			@param UnderHorizonFadingMultiplier Fading multiplier, opacity = saturate(pow(opacity,fading_exp)*fading_multiplier)
		 */
		void setUnderHorizonParams(const int& UnderHorizonCircles = 15, const bool& UnderHorizonFading = true, const Ogre::Real& UnderHorizonFadingExponent = 1, const Ogre::Real& UnderHorizonFadingMultiplier = 2);

		/** Get under-horizon circles
		    @return Under-horizon circles
		 */
		FORCEINLINE const int& getUnderHorizonCircles() const
		{
			return mUnderHorizonCircles;
		}

		/** Get under-horizon fading
		    @return Under-horizon fading
		 */
		FORCEINLINE const bool& getUnderHorizonFading() const
		{
			return mUnderHorizonFading;
		}

		/** Get under-horizon exponent fading
		    @return under-horizon exponent fading
	     */
		FORCEINLINE const Ogre::Real& getUnderHorizonFadingExponent() const
		{
			return mUnderHorizonFadingExponent;
		}

		/** Get under-horizon fading multiplier
		    @return Under-horizon fading multiplier
		 */
		FORCEINLINE const Ogre::Real& getUnderHorizonFadingMultiplier() const
		{
			return mUnderHorizonFadingMultiplier;
		}

		/** Set radius multiplier
		    @param RadiusMultiplier Radius multiplier
			@remarks Radius multiplier in [0,1] range
			         Radius = CameraFarClipDistance * RadiusMultiplier
		 */
		FORCEINLINE void setRadiusMultiplier(const Ogre::Real& RadiusMultiplier)
		{
			mRadiusMultiplier = RadiusMultiplier;
		}

		/** Get skydome radius
		    @param c Camera
		    @return Skydome radius relative to the given camera
		 */
		const float getSkydomeRadius(Ogre::Camera* c) const;


		inline void SetPosition(const Ogre::Vector3& _campos) { if (!mSceneNode) return; mSceneNode->setPosition(_campos); }

	private:
		/** Create geometry
		 */
		void _createGeometry();

		/// Has been create() already called?
		bool mCreated;

		/// Ogre::MeshPtr
        Ogre::Mesh* mMesh;
        /// Ogre::Submesh pointer
        Ogre::SubMesh *mSubMesh;
        /// Ogre::Entity pointer
        Ogre::Entity *mEntity;

        /// Vertex buffer
        Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
        /// Index buffer
        Ogre::HardwareIndexBufferSharedPtr  mIndexBuffer;

		/// Vertices
		VERTEX* mVertices;

		/// Circles
		int mCircles;
		/// Steps
		int mSteps;

		/// Under-horizon rendering
		int mUnderHorizonCircles;
		/// Under-horizon fading
		bool mUnderHorizonFading;
		/// Under-horizon exponent fading (1=linear fading)
		Ogre::Real mUnderHorizonFadingExponent;
		/// Under-horizon fading multiplier: opacity = saturate(pow(opacity,fading_exp)*fading_multiplier)
		Ogre::Real mUnderHorizonFadingMultiplier;

		/// Radius multiplier
		Ogre::Real mRadiusMultiplier;

		/// Ogre::SceneNode pointer
		Ogre::SceneNode* mSceneNode;

        /// Material name
        Ogre::String mMaterialName;

		/// Main SkyX pointer
		SkyX* mSkyX;
	};
}

#endif