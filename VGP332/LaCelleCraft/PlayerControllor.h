#ifndef LACELLECRAFT_GAMEENTITY_PLAYERCONTROLLER_HEADER
#define LACELLECRAFT_GAMEENTITY_PLAYERCONTROLLER_HEADER

#include "Commander.h"
#include "LCTexture.h"

struct PlayerController
{
	Camera2D* mWorldCamera;

	std::list<Unit*> mSeletedUnit;
	std::list<Structure*> mSeletedStructure;

	Unit* mCurrentHighLightUnit = nullptr;
	Structure* mCurrentHighLightStructure = nullptr;
	Structure* mCurrentHighLightResource = nullptr;
	bool mIsHighLight = false;

	bool mIsDragSelecting;
	Vector2 mClickDownPos;
	Vector2 mCurrentMousePos;

	LCTexture mMouseTexture;
	LCTexture mSelectRectTexture;

	bool mIsSelect;
	bool mIsClick = false;

	void Load(Camera2D* worldCamera);

	void Update(float deltaTime);

	void Render(const Camera2D& camera);

	void UIUpdate(float deltaTime, const Camera2D& camera);

	void LateUpdate(float deltaTime);

private:
	void SingleSelecting();
	void MutipleSelecting();
	void ClearSelection();

	void ControlUpdateAllUnit(float deltaTime);
	void ControlUpdateAllBuilding(float deltaTime);
};

#endif // !LACELLECRAFT_GAMEENTITY_PLAYERCONTROLLER_HEADER
