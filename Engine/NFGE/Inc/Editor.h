#ifndef INCLUDE_NFGE_NFGEEDITOR_H
#define INCLUDE_NFGE_NFGEEDITOR_H

namespace NFGE
{
	class GameObject;
	class Service;
	class World;

	class Editor
	{
	public:
		Editor(World& world);

		void ShowMainWindowWithDockSpace();
		void ShowWorldView();
		void ShowInspectorView();
		void ShowMenuBar();
		void ShowSceneView();

		void ShowUtil();

		void CameraControl(NFGE::Graphics::Camera& camera, float deltaTime);

	private:
		World& mWorld;

		Service* mSelectedService = nullptr;
		GameObject* mSelectedGameObject = nullptr;

		bool isShowPop = false;

		void ShowGameObjectInWorldView(GameObject* gameObject, std::string& moveTo, bool& isDraging, bool& isDroped, uint32_t depth = 0);
	};
}

#endif // !INCLUDE_NFGE_NFGEEDITOR_H
