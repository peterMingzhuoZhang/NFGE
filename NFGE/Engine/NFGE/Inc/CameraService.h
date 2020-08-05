#ifndef INCLUDE_NFGE_CAMERASERVICESERVICE_H
#define INCLUDE_NFGE_CAMERASERVICESERVICE_H

#include "Service.h"

namespace NFGE
{
	struct CameraEntry
	{
		META_CLASS_DECLARE;
		CameraEntry()
			:name("NONE")
			, camera()
		{
			Fov = camera.GetFOV() * Math::Constants::RadToDeg;
		}

		explicit CameraEntry(const char* name)
			:name(name)
			,camera()
		{
			Fov = camera.GetFOV() * Math::Constants::RadToDeg;
		}
		std::string name;
		Graphics::Camera camera;

		Math::Vector3 mPosition{ 0.0f };
		Math::Vector3 mDirection{ 0.0f, 0.0f, 1.0f };
		float Fov;
	};

	class CameraService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void WorldViewUI() override;
		void InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*)) override;

		Graphics::Camera* AddCamera(const char* name);
		Graphics::Camera* FindCamera(const char* name);

		void SetActiveCamera(const char* name);
		Graphics::Camera& GetActiveCamera();
		CameraEntry& GetActiveCameraEntry();
		const Graphics::Camera& GetActiveCamera() const;

		// 
		void SetCameraPosition(const char* name, const Math::Vector3& pos);
		void SetCameraDirection(const char* name, const Math::Vector3& dir);
		void SetCameraFOV(const char* name, float pos);

	private:
		std::vector<CameraEntry> mCameraList;
		int mActiveCameraIndex = 0;

		size_t mSelectedCamera = 0;

		size_t GetCameraIndex(const char* name);
		void RefreshWithCameraEntry(size_t camIndex);
	};
}

#endif // !INCLUDE_NFGE_TERRAINSERVICE_H