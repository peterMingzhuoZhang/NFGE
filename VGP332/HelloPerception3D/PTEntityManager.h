#ifndef INCLUDE_HELLOPERCEPTION3D_PTENTITYMANAGER_H
#define INCLUDE_HELLOPERCEPTION3D_PTENTITYMANAGER_H

#include "NFGE/Inc/NFGE.h"
using namespace NFGE;

template <typename T>
class PTEntityManager
{
public:
	static void StaticInitialize(int size, AI::AIWorld3D& world)
	{
		ASSERT(sInstance == nullptr, "[PTEntityManager<%s>] System already initlizlized!", T::GET_TYPE_NAME().c_str());
		sInstance = new PTEntityManager<T>();
		sInstance->Initialize(size, world);
	}
	static void StaticTerminate()
	{
		if (sInstance != nullptr)
		{
			sInstance->Terminate();
			delete sInstance;
		}
	}
	static PTEntityManager* Get()
	{
		ASSERT(sInstance != nullptr, "[PTEntityManager<%s>] System not initlizlized!", T::GET_TYPE_NAME().c_str());
		return sInstance;
	}

	inline static PTEntityManager<T>* sInstance = nullptr;
public:
	PTEntityManager() = default;

	void Initialize(int size, AI::AIWorld3D& world)
	{
		for (int i = 0; i < size; i++)
		{
			mInventory.push_back(new T(world));
		}
	}
	void Terminate()
	{
		for (int i = 0; i < mInventory.size(); i++)
		{
			delete mInventory[i];
		}
	}

	void Update(float deltaTime)
	{
		for (int i = 0; i < mInventory.size(); i++)
		{
			auto ptr = mInventory[i];
			if (!ptr->mIsActive)
				continue;

			ptr->Update(deltaTime);
		}
	}

	void Render()
	{
		for (int i = 0; i < mInventory.size(); i++)
		{
			auto ptr = mInventory[i];
			if (!ptr->mIsActive)
				continue;

			ptr->Render();
		}
	}
	void DebugDraw()
	{
		for (int i = 0; i < mInventory.size(); i++)
		{
			auto ptr = mInventory[i];
			if (!ptr->mIsActive)
				continue;

			ptr->DebugDraw();
		}
	}

	T* GetNext()
	{
		int count = 0;
		auto totalSize = mInventory.size();
		mNext = mNext % totalSize;
		while (count < totalSize)
		{
			if (!mInventory[mNext]->mIsActive)
				return mInventory[mNext++];
			else
				mNext = (mNext + 1) % totalSize;
			++count;
		}
		
		return nullptr;
	}
	T* GetFirstActive()
	{
		for (int i = 0; i < mInventory.size(); i++)
		{
			if (mInventory[i]->mIsActive)
				return mInventory[i];
		}
		return nullptr;
	}

	std::vector<T*>& GetPool() { return mInventory; }
private:
	std::vector<T*> mInventory;
	int mNext = 0;
};
#endif // !INCLUDE_HELLOPERCEPTION3D_PTENTITYMANAGER_H
