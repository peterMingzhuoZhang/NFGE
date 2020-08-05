#ifndef NFGE_LACELLECRAFT_FIGHTABLE_H
#define NFGE_LACELLECRAFT_FIGHTABLE_H

#include "Status.h"

struct Unit;
struct Building;
namespace LC
{
	struct Projectile;

	struct Fightable
	{
	public:
		virtual void Attack(Unit& unit, float deltaTime) = 0;
		virtual void Attack(Building& strcuture, float deltaTime) = 0;
		virtual void TakeDamage(Projectile& bullet) = 0;
		virtual void Die() = 0;
	public:
		std::vector<std::string> mProjectileInventory;
		Status mStatus;
	};
}


#endif // !NFGE_LACELLECRAFT_FIGHTABLE_H
