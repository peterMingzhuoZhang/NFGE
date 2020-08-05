#ifndef INCLUDE_NFGE_GAMEOBJECTFACTORY_H
#define INCLUDE_NFGE_GAMEOBJECTFACTORY_H

#include "GameObject.h"

namespace NFGE
{
	class GameObjectFactory
	{
	public:
		GameObjectFactory(GameObjectAllocator& allocator);

		GameObject* CreateEmpty();
		GameObject* Create(const char* templateFileName);
		void Destory(GameObject* gameObject);

	private:
		GameObjectAllocator& mGameObjectAllocator;
	};
}

#endif // !INCLUDE_NFGE_GAMEOBJECTFACTORY_H

