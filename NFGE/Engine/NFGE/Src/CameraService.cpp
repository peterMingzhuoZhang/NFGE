#include "Precompiled.h"
#include "CameraService.h"
#include "NFGE.h"

using namespace NFGE;

META_DERIVED_BEGIN(CameraService, Service)
	META_NO_FIELD
META_CLASS_END;

META_CLASS_BEGIN(CameraEntry)
	META_FIELD_BEGIN
	META_FIELD(name, "Name")
	META_FIELD(mPosition, "Position")
	META_FIELD(mDirection, "Direction")
	META_FIELD(Fov, "Fov")
	META_FIELD_END
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

void NFGE::CameraService::InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*))
{
	auto& cameraEntry = mCameraList[mSelectedCamera];
	auto metaClass = mCameraList[mSelectedCamera].GetMetaClass();
	ShowMetaClassInInspector(metaClass, (uint8_t*)&(cameraEntry));

	RefreshWithCameraEntry(mSelectedCamera);

	if (ImGui::CollapsingHeader("Control", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool isActive = mSelectedCamera == mActiveCameraIndex;
		if(ImGui::Checkbox("Active Camera", &isActive))
		{
			mActiveCameraIndex = mSelectedCamera;
		}

		if (ImGui::Button("Add Camera", { 200.0f,25.0f }))
		{
			mCameraList.emplace_back(CameraEntry());
			mCameraList.back().name = mCameraList.back().name + "_" + std::to_string(mCameraList.size());
			mSelectedCamera = mCameraList.size() - 1;
		}

		if (mCameraList.size() > 1)
		{
			if (ImGui::Button("Remove Current Camera", { 200.0f,25.0f }))
			{
				mCameraList.erase(mCameraList.begin() + mSelectedCamera);
				mSelectedCamera = 0;
			}
		}
	}

	
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
		entry.name = name;
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

size_t NFGE::CameraService::GetCameraIndex(const char * name)
{
	auto iter = std::find_if(mCameraList.begin(), mCameraList.end(), [name](auto& entry) { return entry.name.c_str(); });
	return (iter == mCameraList.end()) ? UINT_MAX : iter - mCameraList.begin();
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

CameraEntry & NFGE::CameraService::GetActiveCameraEntry()
{
	ASSERT(mActiveCameraIndex < mCameraList.size(), "[CameraService] No active camera!");
	return mCameraList[mActiveCameraIndex];
}

const Graphics::Camera& CameraService::GetActiveCamera() const
{
	ASSERT(mActiveCameraIndex < mCameraList.size(), "[CameraService] No active camera!");
	return mCameraList[mActiveCameraIndex].camera;
}

void NFGE::CameraService::SetCameraPosition(const char* name, const Math::Vector3 & pos)
{
	size_t camIndex = GetCameraIndex(name);
	ASSERT(camIndex != UINT_MAX, "[CameraService] access none existing camera.");
	mCameraList[camIndex].mPosition = pos;
	RefreshWithCameraEntry(camIndex);
}

void NFGE::CameraService::SetCameraDirection(const char* name, const Math::Vector3 & dir)
{
	size_t camIndex = GetCameraIndex(name);
	ASSERT(camIndex != UINT_MAX, "[CameraService] access none existing camera.");
	mCameraList[camIndex].mDirection = dir;
	RefreshWithCameraEntry(camIndex);
}

void NFGE::CameraService::SetCameraFOV(const char* name, float fov)
{
	size_t camIndex = GetCameraIndex(name);
	ASSERT(camIndex != UINT_MAX, "[CameraService] access none existing camera.");
	mCameraList[camIndex].Fov = fov;
	RefreshWithCameraEntry(camIndex);
}

void NFGE::CameraService::RefreshWithCameraEntry(size_t camIndex)
{
	auto& cameraEntry = mCameraList[camIndex];
	cameraEntry.camera.SetPosition(cameraEntry.mPosition);

	if (cameraEntry.mDirection.x == 0.0f && cameraEntry.mDirection.y == 0.0f && cameraEntry.mDirection.z == 0.0f) // TODO:: Override == operator in future
	{
		cameraEntry.mDirection = NFGE::Math::Vector3::One();
	}
	cameraEntry.mDirection = NFGE::Math::Normalize(cameraEntry.mDirection);
	cameraEntry.camera.SetDirection(cameraEntry.mDirection);
	cameraEntry.camera.SetFOV(cameraEntry.Fov * Math::Constants::DegToRad);
}
