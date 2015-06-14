#include"stdafx.h"
#include "wx/generic/filedlgg.h"
#include "tool_t.h"
#include "wx/treebase.h"
#include"resInsertTool_t.h"
#include"frame_t.h"
#include <wx/treectrl.h>
#include "wx/choicebk.h"
#include "wx/splitter.h"
#include "wx/dirctrl.h"
#include "wx/treebook.h"
#include"wx\filename.h"

#include "cgSys_t.h"
#include "xgScene_t.h"
#include "type_t.h"
#include "wx/dnd.h"
#include <string>
#include "pfxType_t.h"
#include "wxOgre_t.h"
#include "dummyColors.h"
#include "xml.h"
using namespace std;


//EditorParts
#include "LightPropertyEditor.h"

namespace vega
{
	static const long idDirCtrl = wxNewId();
	static const long idNoteBookCtl = wxNewId();
	static const long idParticleList = wxNewId();

	static const long idPointLight = wxNewId();
	static const long idDirectionalLight = wxNewId();
	static const long idSpotlightLight = wxNewId();
	static const long idEntityList = wxNewId();

	BEGIN_EVENT_TABLE(resInsertTool_t, wxNotebook)
		EVT_LIST_ITEM_SELECTED(idParticleList, onParticleListEvent)
		EVT_BUTTON(idPointLight, onLightCreateCommanEvent)
		EVT_BUTTON(idDirectionalLight, onLightCreateCommanEvent)
		EVT_BUTTON(idSpotlightLight, onLightCreateCommanEvent)
		EVT_TREE_SEL_CHANGED(wxID_TREECTRL, onEvent)
		EVT_TREE_SEL_CHANGED(idEntityList, onEntityListEvent)
		END_EVENT_TABLE();

	void resInsertTool_t::onLightCreateCommanEvent(wxCommandEvent & event)
	{
		if (event.GetId() == idPointLight)
			m_ResTypeStr = "pointLight";
		else if (event.GetId() == idDirectionalLight)
			m_ResTypeStr = "directionalLight";
		else if (event.GetId() == idSpotlightLight)
			m_ResTypeStr = "spotlightLight";
		update(light_type);
	}

	/**
	*/
	void resInsertTool_t::onParticleListEvent(wxListEvent  & event)
	{
		wxString n = event.GetLabel();
		if (n != m_ResTypeStr) 
		{
			m_ResTypeStr = n;
			update(fx_type);
		}
	}
	void resInsertTool_t::onEvent(wxTreeEvent & event)
	{
		//wxString ws=dir->GetTreeCtrl()->GetItemText(event.GetItem());
		//m_ResTypeStr=dir->GetFilePath ();
		if (dir) {
			wxString n = dir->GetTreeCtrl()->GetItemText(event.GetItem());
			//const char *c=n.c_str();
			if (n != m_ResTypeStr) {
				m_ResTypeStr = n;
				update(mesh_type);
			}
		}
	}
	class entityInfoItemData_t :public wxTreeItemData
	{
	public:
		const string mesh;
		entityInfoItemData_t(const string &mesh)
			:mesh(mesh) {
		}
	};
	void resInsertTool_t::onEntityListEvent(wxTreeEvent & event)
	{
		if (event.GetItem() == entityList->GetRootItem())
		{
			loadEntityMeshPath();
			loadEntityXmlList();
		}
		else if (entityList->GetItemText(event.GetItem()).Right(4) == ".xml" && !entityList->ItemHasChildren(event.GetItem())) {
			loadEntityXmlFile(event.GetItem(), entityList->GetItemText(event.GetItem()));
		}
		else {
			wxTreeItemData *itemData = entityList->GetItemData(event.GetItem());
			if (itemData) {
				entityInfoItemData_t *entityInfoItemData = static_cast<entityInfoItemData_t *>(itemData);
				m_ResTypeStr = entityInfoItemData->mesh;
				gameEntityName = entityList->GetItemText(event.GetItem());
				update(entity_type);
			}
		}
	}
	bool resInsertTool_t::loadEntityXmlFile(const wxTreeItemId &rootId, const wxString &file)
	{
		xmlStack_t x;
		if (!x.loadXMLFile(file.c_str())) {
			string text;
			text += "I am sorry that i can't open file ";
			text += file.c_str();
			text += " for read, may be it is write only , you don't have enough permission or it has been locked by another program";
			MessageBoxA(NULL, text.c_str(), "can't open file for read", MB_OK);
			return false;
		}
		xmlElement_t *root, *t;
		root = x.getChildByName("entityInfoBag");
		if (root) {
			size_t n = root->getChildrenCount();
			string templateName, mesh;
			float scale;
			for (int i = 0; i < n; ++i) {
				t = root->getChild(i);
				if (t && t->getName() == "info") {
					t->getChildByName("base")->get("template", templateName);
					t->getChildByName("entity")->get("mesh", mesh);
					entityInfoItemData_t *objectInfoItemData = new entityInfoItemData_t(mesh);
					entityList->AppendItem(rootId, templateName.c_str(), -1, -1, objectInfoItemData);
					GetEditor()->GetEditorScene()->addEntityInfo(templateName.c_str(), mesh.c_str());
				}
			}
			return true;
		}
		return true;
	}

	wxDirTraverseResult  resInsertTool_t::OnFile(const wxString &filename)
	{
		if (dirTraverseMode == listEntityXmlEnum) {
			wxTreeItemId itemId = entityList->AppendItem(entityList->GetRootItem(), filename);
			loadEntityXmlFile(itemId, filename.c_str());
		}
		return wxDIR_CONTINUE;
	}
	wxDirTraverseResult  resInsertTool_t::OnDir(const wxString &dirname)
	{
		if (dirTraverseMode == listEntityMeshEnum) {
			string path;
			path = GetEditor()->GetEditorScene()->getSubDir(dirname.c_str());
			path += "\\";
			GetEditor()->GetEditorScene()->addResourceLocation(path.c_str());
		}
		return wxDIR_CONTINUE;
	}
	void resInsertTool_t::loadEntityMeshPath()
	{
		if (entityMeshPathLoaded)
			return;
		entityMeshPathLoaded = true;
		dirTraverseMode = listEntityMeshEnum;
		string entityMeshRootDir;
		//	frame->getConfig()->get("entityMeshRootDir",entityMeshRootDir,false);
		//	if(entityMeshRootDir.length()){
		{	wxDir entityRootDir(entityMeshRootDir.c_str());
		if (wxDir::Exists(entityMeshRootDir.c_str())) {
			dirTraverseMode = listEntityMeshEnum;
			entityRootDir.Traverse(*this, "", wxDIR_DIRS | wxDIR_HIDDEN);
		}
		}
	}

	void resInsertTool_t::loadEntityXmlList()
	{
		if (entityXmlListLoaded)
			return;
		entityXmlListLoaded = true;
		string entityXmlRootDir;
		//	frame->getConfig()->get("entityXmlRootDir",entityXmlRootDir,false);
		//	if(entityXmlRootDir.length()){
		{
			wxDir entityRootDir(entityXmlRootDir.c_str());
			if (wxDir::Exists(entityXmlRootDir.c_str()))
			{
				dirTraverseMode = listEntityXmlEnum;
				entityRootDir.Traverse(*this, "*.xml");
			}
		}
	}

	resInsertTool_t::resInsertTool_t()
	{
		entityXmlListLoaded = false;
		entityMeshPathLoaded = false;
		entity = NULL;
		sceneNode = NULL;
		id = 0;
	}

	void resInsertTool_t::update(int resType)
	{
		currentResType = resType;
		/*if (sceneNode && entity)
		{
			GetEditor()->GetEditorScene()->setSelSceneNode(NULL);
			GetEditor()->GetEditorScene()->setSelMovable(NULL);
			GetEditor()->GetEditorScene()->destroySceneNode(sceneNode->getName());
			GetEditor()->GetEditorScene()->destroyMovableObject(entity->getName(), entity->_getCreator()->getType());
			entity = NULL;
			sceneNode = NULL;
		}*/
		if (active)
		{
			try
			{
				if (resType == mesh_type && m_ResTypeStr.Right(5) == ".mesh")
				{
					string path;
					try
					{
						wxString filePath = dir->GetFilePath();
						wxString resPath = filePath.Left(filePath.Len() - m_ResTypeStr.Len());
						path = GetEditor()->GetEditorScene()->getSubDir(resPath.c_str());
						GetEditor()->GetEditorScene()->addResourceLocation(path.c_str());
					}
					catch (Ogre::Exception& e)
					{
						std::cerr << "An exception has occured: " << e.getFullDescription();
						MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
					}

					if (Ogre::ResourceGroupManager::getSingleton().resourceExists(path, m_ResTypeStr.c_str().AsChar())) {
						xgstring name;
						name.printf("%s_%d", m_ResTypeStr.c_str(), id++);
						while (GetEditor()->GetEditorScene()->hasEntity(name.c_str())) {
							name.printf("%s_%d", m_ResTypeStr.c_str(), id++);
						}
						Ogre::Entity *e = GetEditor()->GetEditorScene()->createEntity(name.c_str(), m_ResTypeStr.c_str().AsChar());
						entity = e;
						entity->setUserAny(*new type_t(mesh_type_));
						/*if (entity) 
						{
							sceneNode = GetEditor()->GetEditorScene()->createSceneNode(name.c_str());
							if (sceneNode) {
								sceneNode->attachObject(entity);
								GetEditor()->GetEditorScene()->getRootSceneNode()->addChild(sceneNode);
							}
						}*/
					}
				}
				else if (resType == fx_type) {
					xgstring name;
					name.printf("%s_%d", m_ResTypeStr.c_str(), id++);
					while (GetEditor()->GetEditorScene()->hasParticleSystem(name.c_str())) {
						name.printf("%s_%d", m_ResTypeStr.c_str(), id++);
					}
					Ogre::ParticleSystem *pfx = GetEditor()->GetEditorScene()->createParticleSystem(name.c_str(), m_ResTypeStr.c_str().AsChar());
					entity = pfx;
					pfx->setBoundsAutoUpdated(false);
					entity->setUserAny(*new pfxType_t(fx_type_, m_ResTypeStr.c_str()));
					/*if (entity) 
					{
						sceneNode = GetEditor()->GetEditorScene()->createSceneNode(name.c_str());
						if (sceneNode) {
							Ogre::ManualObject *dummy = GetEditor()->GetEditorScene()->createBoxManualObject(GetEditor()->GetEditorScene()->getHelpMeshName(), PFX_COLOR, Ogre::Vector3(-10, -10, -10), Ogre::Vector3(10, 10, 10), "pfxMesh");
							dummy->setUserAny(*new type_t(dummy_type_));
							sceneNode->attachObject(dummy);

							sceneNode->attachObject(entity);
							GetEditor()->GetEditorScene()->getRootSceneNode()->addChild(sceneNode);
						}
					}*/
				}
				//Nick:переписал  
				else if (resType == light_type)
				{
					Ogre::Vector3 dir(0, -1, 0);
					dir.normalise();
					vega::ActorLight* light = nullptr;

					if (m_ResTypeStr == "pointLight")
						light = vega::ActorLight::PointLight(m_ResTypeStr.c_str().AsChar());
					else if (m_ResTypeStr == "directionalLight")
						light = vega::ActorLight::DirectLight(m_ResTypeStr.c_str().AsChar());
					else if (m_ResTypeStr == "spotlightLight")
						light = vega::ActorLight::SpotLight(m_ResTypeStr.c_str().AsChar());

					GetPropEditors().m_LightPE->SetEditableLight(light);

					/*light->getOgreLight()->setUserAny(*new type_t(light_type_));*/
					light->setDirection(dir);
					light->setDiffuse(Ogre::ColourValue(0.6, 0.6, 0.6));
					light->setPowerScale(50, 1, 0, 0);
					Ogre::ManualObject *dummy = GetEditor()->GetEditorScene()->createBoxManualObject(GetEditor()->GetEditorScene()->getHelpMeshName(), LIGHT_COLOR, Ogre::Vector3(-10, -10, -10), Ogre::Vector3(10, 10, 10), "LightMesh");
					dummy->setUserAny(*new type_t(dummy_type_));
					light->getNode()->attachObject(dummy);


				}
				//end
				else if (resType == entity_type)
				{
					if (m_ResTypeStr.Right(5) == ".mesh")
					{
						Ogre::String path = Ogre::ResourceGroupManager::getSingleton().findGroupContainingResource(m_ResTypeStr.c_str().AsChar());
						if (Ogre::ResourceGroupManager::getSingleton().resourceExists(path, m_ResTypeStr.c_str().AsChar())) {
							xgstring name;
							name.printf("%s_%d", m_ResTypeStr.c_str(), id++);
							while (GetEditor()->GetEditorScene()->hasEntity(name.c_str())) {
								name.printf("%s_%d", m_ResTypeStr.c_str(), id++);
							}
							Ogre::Entity *e = GetEditor()->GetEditorScene()->createEntity(name.c_str(), m_ResTypeStr.c_str().AsChar());
							entity = e;
							type_t *t = new type_t(entity_type_);
							t->userData = gameEntityName;
							entity->setUserAny(*t);
							if (entity)
							{
								sceneNode = GetEditor()->GetEditorScene()->createSceneNode(name.c_str());
								if (sceneNode) {
									sceneNode->attachObject(entity);
									GetEditor()->GetEditorScene()->getRootSceneNode()->addChild(sceneNode);
								}
							}
						}
					}
				}
				if (sceneNode)
				{
					GetEditor()->GetEditorScene()->setSelSceneNode(sceneNode);
					GetEditor()->GetEditorScene()->setSelMovable(entity);
				}
			}
			catch (Ogre::Exception& e)
			{
				std::cerr << "An exception has occured: " << e.getFullDescription();
				MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
			}
		}
	}

	bool resInsertTool_t::onActive() {
		frame->setViewMsgRecvTool(this);
		active = true;
		update(currentResType);
		return true;
	}

	bool resInsertTool_t::onDeactive() {
		frame->setViewMsgRecvTool(NULL);
		active = false;
		update(currentResType);
		return true;
	}

	bool resInsertTool_t::onViewEvent(wxEvent & event) {
		return false;
	}

	bool resInsertTool_t::Show(bool show) {
		if (show) {
			onActive();
		}
		else {
			onDeactive();
		}
		return wxNotebook::Show(show);
	}

	class DnDFile : public wxFileDropTarget {
		resInsertTool_t *resInsertTool;
	public:
		DnDFile(resInsertTool_t *resInsertTool) :resInsertTool(resInsertTool) {}
		virtual bool OnDropFiles(wxCoord x, wxCoord y,
			const wxArrayString& filenames) {
			size_t nFiles = filenames.GetCount();
			wxString str;
			str.Printf(_T("%d files dropped"), (int)nFiles);
			cout << str.c_str() << "\n";
			for (size_t n = 0; n < nFiles; n++) {
				cout << filenames[n] << " dropped\n";
				resInsertTool->insertRes(filenames[n], x, y);
			}
			return false;
		}
	};

	bool resInsertTool_t::insertRes(const char *file, int x, int y) {
		wxFileName  fileName(file);
		if (fileName.DirExists(file)) {
			string path = GetEditor()->GetEditorScene()->getSubDir(file);
			if (GetEditor()->GetEditorScene()->addResourceLocation(path.c_str())) {
				updateParticleList();
			}
		}
		else {
			string name = file;
			if (name.length() > 5 && name.substr(name.length() - 5, 5) == ".mesh") {
				basic_string <char>::size_type indexCh1a;
				static const basic_string <char>::size_type npos = -1;
				indexCh1a = name.rfind("\\");
				string path;
				if (indexCh1a != npos) {
					string resName = name.substr(indexCh1a + 1, name.length() - indexCh1a - 1);
					path = name.substr(0, indexCh1a + 1);
					path = GetEditor()->GetEditorScene()->getSubDir(path.c_str());
					if (GetEditor()->GetEditorScene()->addResourceLocation(path.c_str())) {
						updateParticleList();
					}
					if (Ogre::ResourceGroupManager::getSingleton().resourceExists(path, resName)) {
						wxString name;
						name.Printf("%s_%d", resName.c_str(), id++);
						while (GetEditor()->GetEditorScene()->hasEntity(name.c_str().AsChar())) {
							name.Printf("%s_%d", resName.c_str(), id++);
						}
						Ogre::Entity *e = GetEditor()->GetEditorScene()->createEntity(name.c_str().AsChar(), resName.c_str());
						entity = e;
						//e->setDisplaySkeleton(true);
						entity->setUserAny(*new type_t(mesh_type_));
						if (entity) {
							sceneNode = GetEditor()->GetEditorScene()->createSceneNode(name.c_str().AsChar());
							if (sceneNode) {
								sceneNode->attachObject(entity);
								GetEditor()->GetEditorScene()->getRootSceneNode()->addChild(sceneNode);
								Ogre::Vector3 pos;
								if (getIntersectGroundPoint(x, y, pos)) {
									sceneNode->setPosition(pos);
								}
							}
						}
					}
				}
				return true;
			}
		}
		return false;
	}

	bool resInsertTool_t::getIntersectGroundPoint(int x, int y, Ogre::Vector3 &pos) {
		Ogre::Viewport *vp = GetEditor()->GetEditorScene()->getCurrentViewport();
		Ogre::Camera *c = vp->getCamera();
		Ogre::RenderTarget *rt = vp->getTarget();
		float tgrx = x / (float)rt->getWidth();
		float tgry = y / (float)rt->getHeight();
		if (tgrx<vp->getLeft() || tgrx>vp->getLeft() + vp->getWidth() ||
			tgry<vp->getTop() || tgry>vp->getTop() + vp->getHeight())
			return false;
		float tscreenx = (tgrx - vp->getLeft()) / vp->getWidth();
		float tscreeny = (tgry - vp->getTop()) / vp->getHeight();
		Ogre::Ray ray = c->getCameraToViewportRay(tscreenx, tscreeny);
		Ogre::Plane plane(Ogre::Vector3(0, 1, 0), 0);
		std::pair< bool, Ogre::Real > interPoint = Ogre::Math::intersects(ray, plane);
		if (interPoint.first) {
			pos = ray.getPoint(interPoint.second);
			return true;
		}
		return false;
	}

	bool resInsertTool_t::onViewMouseEvent(wxMouseEvent & event) {
		if (!sceneNode)
			return false;
		Ogre::Vector3 pos;
		long x, y;
		event.GetPosition(&x, &y);
		if (getIntersectGroundPoint(x, y, pos))
			sceneNode->setPosition(pos);
		if (event.ButtonUp(wxMOUSE_BTN_LEFT)) {
			entity = NULL;
			sceneNode = NULL;
			update(currentResType);
		}

		return true;
	}

	class wxGenericDirCtrlEx :public wxGenericDirCtrl {
		DECLARE_EVENT_TABLE()
		resInsertTool_t *me;
	public:
		void onEvent(wxTreeEvent & event) {
			me->onEvent(event);
		}
		wxGenericDirCtrlEx(wxWindow *parent, const wxWindowID id = wxID_ANY,
			const wxString &dir = wxDirDialogDefaultFolderStr,
			resInsertTool_t *me = NULL) :wxGenericDirCtrl(parent, id, dir), me(me) {
		}
	};

	BEGIN_EVENT_TABLE(wxGenericDirCtrlEx, wxGenericDirCtrl)
		EVT_TREE_SEL_CHANGED(wxID_TREECTRL, onEvent)
		END_EVENT_TABLE();

	bool resInsertTool_t::init() 
	{
		frame->getView()->SetDropTarget(new DnDFile(this));
		string resInsertToolDirInitDir;
		wxNotebook *book = this;
		if (!Create(frame->getToolBook(), idNoteBookCtl)) {
			return false;
		}
		dir = NULL;
		dir = new wxGenericDirCtrl(book, idDirCtrl, resInsertToolDirInitDir.c_str());
		book->AddPage(dir, "mesh");
		particleList = new wxListCtrl(book, idParticleList, wxDefaultPosition, wxDefaultSize, wxLC_LIST);

		book->AddPage(particleList, "particle");

		entityList = new wxTreeCtrl(book, idEntityList);
		book->AddPage(entityList, "entity");
		wxTreeItemId entityRootId = entityList->AddRoot("entity");


		wxWindow *lightWindow = new wxPanel(book, wxID_ANY);

		wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
		box->Add(new wxButton(lightWindow, idPointLight, "point light"), 0, wxGROW | wxTOP, 2);
		box->Add(new wxButton(lightWindow, idDirectionalLight, "directional light"), 0, wxGROW | wxTOP, 2);
		box->Add(new wxButton(lightWindow, idSpotlightLight, "spotlight light"), 0, wxGROW | wxTOP, 2);

		lightWindow->SetSizer(box);
		box->Fit(lightWindow);
		box->SetSizeHints(lightWindow);
		//box->Layout();


		book->AddPage(lightWindow, "light");
		frame->getToolBook()->AddPage(book, "resource explorer");
		updateParticleList();

		loadEntityMeshPath();
		loadEntityXmlList();

		return true;
	}

	resInsertTool_t::~resInsertTool_t() {
	}

	void resInsertTool_t::refresh() {}

	void resInsertTool_t::updateParticleList() {
		Ogre::ParticleSystemManager::ParticleSystemTemplateIterator t = Ogre::ParticleSystemManager::getSingleton().getTemplateIterator();
		particleList->ClearAll();
		int i(0);
		while (t.hasMoreElements()) {
			particleList->InsertItem(i++, t.peekNextKey());
			t.moveNext();
		}
	}
}