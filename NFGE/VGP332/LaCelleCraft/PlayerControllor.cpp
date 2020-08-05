#include "PlayerControllor.h"
#include "LevelManager.h"

using namespace NFGE::Input;

namespace
{
	std::string mouseTextureDir = "LaCelleCraft/Util/mouse.png";
	std::string selectedRectTextureDir = "LaCelleCraft/Util/selectedRect.png";
}

void PlayerController::Load(Camera2D* worldCamera)
{
	mWorldCamera = worldCamera;
	mSelectRectTexture.Load({ 32.0f,32.0f }, {0.0f, 0.0f}, selectedRectTextureDir);
	mMouseTexture.Load({ 32.0f,32.0f }, { 0.0f, 0.0f }, mouseTextureDir);
	mIsSelect = false;
}

void PlayerController::Update(float deltaTime)
{
	mCurrentMousePos = { (float)GetMouseScreenX(), (float)GetMouseScreenY()};
	mIsHighLight = false;

	if (IsMouseDown(MouseButton::LBUTTON))
	{
		mIsSelect = false;
		mIsClick = true;
		mIsDragSelecting = true;
	}
	else
	{
		mIsDragSelecting = false;
	}

	if (mIsDragSelecting)
	{
		mSelectRectTexture.ChangeSize(mCurrentMousePos - mClickDownPos);
	}

	if ((mIsClick)&&(!mIsDragSelecting))
	{
		mIsClick = false;
		if ((mSelectRectTexture.mDrawSize.x <= 1.0f) && (mSelectRectTexture.mDrawSize.y <= 1.0f))
		{
			SingleSelecting();
		}
		else
		{
			MutipleSelecting();
		}
	}

	if (!mIsDragSelecting)
	{
		mClickDownPos = mCurrentMousePos;
	}


	ControlUpdateAllUnit(deltaTime);
	ControlUpdateAllBuilding(deltaTime);
}

void PlayerController::Render(const Camera2D & camera)
{
	if (mIsDragSelecting)
	{
		mSelectRectTexture.Render(mClickDownPos, camera, true);
		Rect selectRect =
		{
			mSelectRectTexture.mCurrentRect.left + mClickDownPos.x,
			mSelectRectTexture.mCurrentRect.top + mClickDownPos.y,
			mSelectRectTexture.mCurrentRect.right + mClickDownPos.x,
			mSelectRectTexture.mCurrentRect.bottom + mClickDownPos.y,
		};
		DrawScreenRect(selectRect, Green);
	}

	mMouseTexture.Render(mCurrentMousePos, camera, true);
}

void PlayerController::UIUpdate(float deltaTime, const Camera2D & camera)
{
	if (mIsSelect)
	{
		if (!mSeletedUnit.empty())
			mSeletedUnit.front()->Interact();
		else
			mSeletedStructure.front()->Interact();
	}

	ImGui::Begin("Control Center");
	ImGui::Text("SelectedUnit: %d", mSeletedUnit.size());
	ImGui::Text("SelectedStructure: %d", mSeletedStructure.size());
	ImGui::End();
}

void PlayerController::LateUpdate(float deltaTime)
{
	if (mIsHighLight == false)
	{
		mCurrentHighLightUnit = nullptr;
		mCurrentHighLightStructure = nullptr;
		mCurrentHighLightResource = nullptr;
	}
}

void PlayerController::SingleSelecting()
{
	ClearSelection();

	auto levelManager = LevelManager::Get();
	auto& unitTargets = levelManager->mCommanders[levelManager->mLocalCommandersIndex].myUnit;

	bool hasSelect = false;
	for (auto& unit : unitTargets)
	{
		if (unit->Selecting(mCurrentMousePos,*mWorldCamera))
		{
			mSeletedUnit.push_back(unit);
			hasSelect = true;
			mIsSelect = true;
			break;
		}
	}

	if (hasSelect)
		return;

	auto& buildTargets = levelManager->mCommanders[levelManager->mLocalCommandersIndex].myBuilding;
	for (auto& building : buildTargets)
	{
		if (building->Selecting(mCurrentMousePos, *mWorldCamera))
		{
			mSeletedStructure.push_back(building);
			hasSelect = true;
			mIsSelect = true;
			break;
		}
	}

	if (hasSelect)
		return;

	auto& resourceTargets = levelManager->myBattleMapPtr->mAllResource;
	for (auto& resource : resourceTargets)
	{
		if (resource.Selecting(mCurrentMousePos, *mWorldCamera))
		{
			mSeletedStructure.push_back(&resource);
			mIsSelect = true;
			break;
		}
	}
}

void PlayerController::MutipleSelecting()
{
	ClearSelection();
	Rect selectRect =
	{
		mSelectRectTexture.mCurrentRect.left + mClickDownPos.x,
		mSelectRectTexture.mCurrentRect.top + mClickDownPos.y,
		mSelectRectTexture.mCurrentRect.right + mClickDownPos.x,
		mSelectRectTexture.mCurrentRect.bottom + mClickDownPos.y,
	};

	auto levelManager = LevelManager::Get();
	auto& unitTargets = levelManager->mCommanders[levelManager->mLocalCommandersIndex].myUnit;

	bool hasSelect = false;
	for (auto& unit : unitTargets)
	{
		if (unit->Selecting(selectRect, *mWorldCamera))
		{
			mSeletedUnit.push_back(unit);
			hasSelect = true;
			mIsSelect = true;
		}
	}

	if (hasSelect)
		return;

	auto& buildTargets = levelManager->mCommanders[levelManager->mLocalCommandersIndex].myBuilding;
	for (auto& building : buildTargets)
	{
		if (building->Selecting(selectRect, *mWorldCamera))
		{
			mSeletedStructure.push_back(building);
			hasSelect = true;
			mIsSelect = true;
		}
	}

	

}

void PlayerController::ClearSelection()
{
	if (!mSeletedUnit.empty())
	{
		for (auto& unit : mSeletedUnit)
		{
			unit->DeSelecting();
		}
		mSeletedUnit.clear();
	}

	if (!mSeletedStructure.empty())
	{
		for (auto& structure : mSeletedStructure)
		{
			structure->DeSelecting();
		}
		mSeletedStructure.clear();
	}

}

void PlayerController::ControlUpdateAllUnit(float deltaTime)
{
	if (!mSeletedUnit.empty())
	{
		for (auto& unit : mSeletedUnit)
		{
			unit->ControlUpdate(mCurrentMousePos, deltaTime);
		}
	}
}

void PlayerController::ControlUpdateAllBuilding(float deltaTime)
{
	if (!mSeletedStructure.empty())
	{
		for (auto& structure : mSeletedStructure)
		{
			//structure->ControlUpdate(deltaTime);
		}
	}
}
