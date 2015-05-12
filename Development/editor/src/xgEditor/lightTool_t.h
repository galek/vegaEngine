#pragma once
class wxGenericDirCtrl;

namespace vega
{
	class lightTool_t : public tool_t{
		DECLARE_EVENT_TABLE()
		bool active;
		ManualObject *manualObject;
		SceneNode *sceneNode;
		Vector3 oldPoint;
		Vector3 startPoint;
		Vector3 endPoint;
		int state;
		int id;
		wxCoord oldMouseYPos;
		Vector3 update();
		void makeTrangle(const Vector3 *p, int a, int b, int c);
	public:
		lightTool_t();
		~lightTool_t();
		virtual bool init();
		//##ModelId=44B8E4980203
		void onEvent(wxCommandEvent & event);
		virtual bool onActive();
		//##ModelId=44C0D84901C5
		virtual bool onDeactive();
		virtual bool onViewMouseEvent(wxMouseEvent & event);
	};
}