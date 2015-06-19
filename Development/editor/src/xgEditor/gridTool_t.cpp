#include"stdafx.h"
#include "tool_t.h"
#include"gridTool_t.h"
#include "cgSys_t.h"
#include "xgScene_t.h"
#include "type_t.h"

namespace vega
{
	bool gridTool_t::init()
	{
#if 0
		grid = GetEditor()->GetEditorScene()->createManualObject("marke");
		grid->setUserAny(*new type_t(grid_helper_type_));
		grid->begin("gridMesh", Ogre::RenderOperation::OT_LINE_LIST);
		float xwidth;
		float yheight;
		float gup;
		xwidth = 1000;
		yheight = 1000;
		gup = 20;
		xwidth /= 2;
		yheight /= 2;
		Ogre::ColourValue gray(0.5f, 0.5f, 0.5f);
		Ogre::ColourValue black(0.31f, 0.31f, 0.31f);
		for (float x = -xwidth; x < xwidth + 1; x += gup)
		{
			grid->position(x, 0, -yheight);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			grid->colour(gray);

			grid->position(x, 0, yheight);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			grid->colour(gray);

		}
		for (float z = -yheight; z < yheight + 1; z += gup)
		{
			grid->position(-xwidth, 0, z);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			grid->colour(gray);

			grid->position(xwidth, 0, z);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			grid->colour(gray);
		}
		grid->position(0, 0, -yheight);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(black);

		grid->position(0, 0, yheight);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(black);

		grid->position(-xwidth, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(black);

		grid->position(xwidth, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(black);

		grid->position(0, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(Ogre::ColourValue(0, 0, 1, 1));

		grid->position(0, 0, 10);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(Ogre::ColourValue(0, 0, 1, 1));

		grid->position(0, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(Ogre::ColourValue(1, 0, 0, 1));

		grid->position(10, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(Ogre::ColourValue(1, 0, 0, 1));

		grid->position(0, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(Ogre::ColourValue(0, 1, 0, 1));

		grid->position(0, 10, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(Ogre::ColourValue(0, 1, 0, 1));

		grid->end();
		Ogre::SceneManager *sm = Ogre::Root::getSingletonPtr()->getSceneManager("xgSceneInstance");
		Ogre::SceneNode *mk = sm->createSceneNode("gridMesh");
		sm->getRootSceneNode()->addChild(mk);
		mk->attachObject(grid);
#endif
		return true;
	}

	bool gridTool_t::onUiEvent(wxEvent & event){
		return true;
	}

	gridTool_t::~gridTool_t(){
#if 0
		GetEditor()->GetEditorScene()->destroyManualObject(grid);
#endif
	}
}