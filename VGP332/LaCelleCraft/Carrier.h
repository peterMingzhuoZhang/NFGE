#ifndef LACELLECRAFT_GAMEOBJECT_CARRIR_HEADER
#define LACELLECRAFT_GAMEOBJECT_CARRIR_HEADER

#include "Unit.h"
#include "Projectile.h"

struct Interceptor;

struct Carrier : Unit
{
	RTTI(Carrier)

	static TextureId mDefaultTexture[UnitDefaultDirectionCount];
	static int mHealthBarSliceCount;

	std::unique_ptr<AI::StateMachine<Carrier>> stateMachine;

	std::list<Interceptor*> mInterceptors;

	Carrier();
	void Load(TextureGridMap& worldMap, const Vector2& initPos, int commanderIndex);
	void Update(float deltaTime);
	void ControlUpdate(const Vector2& currentMousePos, float deltaTime) override;
	void Render(const Camera2D& camera) override;
	void DebugUIUpdate(float deltaTime, const Camera2D& camera) override;
	void Unload();

	void Interact() override
	{

	}

	TextureId GetDefaultTexture(int index = 0) override
	{
		return mDefaultTexture[0];
	}

	void Attack( Unit& unit, float deltaTime) override;
	void Attack( Building& strcuture, float deltaTime) override;
	void TakeDamage(LC::Projectile& bullet) override;
	void Die() override;
};

#endif // !LACELLECRAFT_GAMEOBJECT_CARRIR_HEADER