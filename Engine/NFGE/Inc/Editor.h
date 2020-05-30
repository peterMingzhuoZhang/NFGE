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
	};
}

#endif // !INCLUDE_NFGE_NFGEEDITOR_H
