#ifndef INCLUDE_NFGE_CAMERASERVICESERVICE_H
#define INCLUDE_NFGE_CAMERASERVICESERVICE_H

#include "Service.h"

namespace NFGE
{
	class CameraService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void WorldViewUI() override;
		void DebugUI() override;

		Graphics::Camera* AddCamera(const char* name);
		Graphics::Camera* FindCamera(const char* name);

		void SetActiveCamera(const char* name);
		Graphics::Camera& GetActiveCamera();
		const Graphics::Camera& GetActiveCamera() const;

	private:
		struct CameraEntry
		{
			explicit CameraEntry(const char* name)
				:name(name)
			{

			}
			std::string name;
			Graphics::Camera camera;
			
			Math::Vector3 mPosition { 0.0f };
			Math::Vector3 mDirection{ 0.0f, 0.0f, 1.0f };
			float Fov;
		};

		std::vector<CameraEntry> mCameraList;
		int mActiveCameraIndex = 0;

		size_t mSelectedCamera = 0;
	};
}

#endif // !INCLUDE_NFGE_TERRAINSERVICE_H