#ifndef LACELLECRAFT_GAMEOBJECT_INTERCEPTOR_HEADER
#define LACELLECRAFT_GAMEOBJECT_INTERCEPTOR_HEADER

#include "Unit.h"

struct Carrier;

struct Interceptor : Unit
{
	RTTI(Interceptor)

	static TextureId mDefaultTexture[UnitDefaultDirectionCount];
	static int mHealthBarSliceCount;

	std::unique_ptr<AI::StateMachine<Interceptor>> stateMachine;

	bool mIsInDock = true;
	std::list<Vector2> mTrivalPoints;
	Carrier* mMotherShip;

	Interceptor();
	void Load(TextureGridMap& worldMap, const Vector2& initPos, Carrier* motherShip, int commanderIndex);
	void Update(float deltaTime);
	void ControlUpdate(const Vector2& currentMousePos, float deltaTime) override;
	void Render(const Camera2D& camera) override;
	void DebugUIUpdate(float deltaTime, const Camera2D& camera) override;
	void Unload();

	void Interact() override
	{
		if (mIsActive || mIsInDock)
			return;
	}

	TextureId GetDefaultTexture(int index = 0)
	{
		return mDefaultTexture[0];
	}

	void Attack( Unit& unit, float deltaTime) override;
	void Attack( Building& strcuture, float deltaTime) override;
	void TakeDamage(Projectile& bullet) override;
	void Die() override;
};

#endif // !LACELLECRAFT_GAMEOBJECT_INTERCEPTOR_HEADER