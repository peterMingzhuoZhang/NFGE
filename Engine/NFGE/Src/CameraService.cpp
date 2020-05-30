#include "Precompiled.h"
#include "CameraService.h"

using namespace NFGE;

META_DERIVED_BEGIN(CameraService, Service)
	META_NO_FIELD
META_CLASS_END;

void NFGE::CameraService::WorldViewUI()
{
	if (ImGui::TreeNode("Cameras"))
	{
		for (size_t i = 0; i < mCameraList.size(); ++i)
		{
			auto& camera = mCameraList[i];

			if (ImGui::Selectable(camera.name.c_str(), mSelectedCamera == i))
			{
				mSelectedCamera = i;
			}
		}
		ImGui::TreePop();
	}
	
}

void CameraService::DebugUI()
{
	

}

Graphics::Camera* CameraService::AddCamera(const char* name)
{
	// Hack
	if (mCameraList.empty())
	{
		mCameraList.reserve(10);
	}

	auto camera = FindCamera(name);
	if (camera == nullptr)
	{
		auto& entry = mCameraList.emplace_back(name);
		camera = &entry.camera;
	}
	return camera;
}

Graphics::Camera* CameraService::FindCamera(const char* name)
{
	auto iter = std::find_if(mCameraList.begin(), mCameraList.end(), [name](auto& entry)
	{
		return entry.name.c_str();
	});
	return (iter == mCameraList.end()) ? nullptr : &iter->camera;
}

void CameraService::SetActiveCamera(const char* name)
{
	for (size_t i = 0; i < mCameraList.size(); ++i)
	{
		auto& entry = mCameraList[i];
		if (entry.name == name)
		{
			mActiveCameraIndex = static_cast<int>(i);
			break;
		}
	}
}

Graphics::Camera& CameraService::GetActiveCamera()
{
	ASSERT(mActiveCameraIndex < mCameraList.size(), "[CameraService] No active camera!");
	return mCameraList[mActiveCameraIndex].camera;
}

const Graphics::Camera& CameraService::GetActiveCamera() const
{
	ASSERT(mActiveCameraIndex < mCameraList.size(), "[CameraService] No active camera!");
	return mCameraList[mActiveCameraIndex].camera;
}