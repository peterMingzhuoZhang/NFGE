#ifndef LACELLECRAFT_GAMEOBJECT_PCLWORKER_HEADER
#define LACELLECRAFT_GAMEOBJECT_PCLWORKER_HEADER

#include "Worker.h"

namespace LCUnit
{

	struct PCLWorker : Worker
	{
		RTTI(PCLWorker)

		static TextureId mDefaultTexture[UnitDefaultDirectionCount];

		PCLWorker();

		void Interact() override
		{

		}

		TextureId GetDefaultTexture(int index = 0) override
		{
			return mDefaultTexture[0];
		}

		void Load(TextureGridMap& worldMap, const Vector2& initPos, int commanderIndex);

		void Attack(Unit& unit, float deltaTime) override;
		void Attack(Building& strcuture, float deltaTime) override;
		void TakeDamage(LC::Projectile& bullet) override;
		void Die() override;
	};

}
#endif // !LACELLECRAFT_GAMEOBJECT_PCLWORKER_HEADER
