#include "Precompiled.h"
#include "World.h"
#include "NFGE.h"

#include "GameObjectFactory.h"
#include "TransformComponent.h"

using namespace NFGE;

void World::LoadLevel(const std::filesystem::path &  filename)
{
	using namespace rapidjson;

	FILE* file = nullptr;
	fopen_s(&file, filename.u8string().c_str(), "r");

	char readBuffer[65536];
	FileReadStream is(file, readBuffer, sizeof(readBuffer));

	Document document;
	document.ParseStream(is);

	if (document.HasMember("Services") && document["Services"].IsObject())
	{
		auto services = document["Services"].GetObjectW();

		
			for (auto& service : services)
			{
				auto metaClass = Core::Meta::FindMetaClass(service.name.GetString());
				auto newService = AddService(metaClass);
				ASSERT(newService, "[World] Failed to create service %s.", service.name.GetString());
				metaClass->Deserialize(newService, service.value);
			}
	}
	if (document.HasMember("GameObjects") && document["GameObjects"].IsArray())
	{

	}
}

void World::Initialize(size_t capacity)
{
	ASSERT(!mInitialized, "[World] World already initialized.");

	for (auto& service : mServices)
		service->Initialize();

	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);

	//mTerrain.Initalize()
	NFGE::sApp.SetWorld(*this);

	mInitialized = true;
}
void World::Terminate()
{
	if (!mInitialized)
		return;

	ASSERT(!mUpdating, "[World] Cannnot terminate world during update.");

	// Destroy all active objects
	mUpdating = true;
	std::for_each(mUpdateList.begin(), mUpdateList.end(), [this](auto gameObject)
	{
		Destroy(gameObject->GetHandle());
	});
	mUpdating = false;
	mUpdateList.clear();

	// Now destroy everything
	ProcessDestroyList();

	mGameObjectAllocator.reset();
	mGameObjectHandlePool.reset();
	mGameObjectFactory.reset();

	for (auto& service : mServices)
		service->Terminate();

	mInitialized = false;
}

Service* World::AddService(const Core::Meta::MetaClass* metaClass)
{
	ASSERT(!mInitialized, "[World] Cannot add servive after world has already initialized. ");
	Service* newService = static_cast<Service*>(metaClass->Create());
	newService->mWorld = this;
	mServices.emplace_back(std::unique_ptr<Service>(newService));
	return newService;
}

GameObjectHandle NFGE::World::CreateEmpty(std::string name, GameObject * parent)
{
	auto gameObject = mGameObjectFactory->CreateEmpty();
	ASSERT(gameObject != nullptr, "[World] Failed to create empty game object");

	// Register withe the handle pool
	auto handle = mGameObjectHandlePool->Register(gameObject);

	gameObject->mWorld = this;
	gameObject->mHandle = handle;
	gameObject->mName = std::move(name);
	gameObject->AddComponent<NFGE::TransformComponent>();
	
	gameObject->Initialize();

	if (parent)
	{
		gameObject->mParent = parent;
		parent->mChilds.push_back(gameObject);
	}

	// Add game object to the update list
	mUpdateList.push_back(gameObject);

	return handle;
}

GameObjectHandle World::Create(const std::filesystem::path& templateFileName, std::string name, GameObject* parent)
{
	auto gameObject = mGameObjectFactory->Create(templateFileName.u8string().c_str());
	ASSERT(gameObject != nullptr, "[World] Failed to create game object from template %s", templateFileName.u8string().c_str());

	// Register withe the handle pool
	auto handle = mGameObjectHandlePool->Register(gameObject);

	// Initialize the game object
	gameObject->mWorld = this;
	gameObject->mHandle = handle;
	gameObject->mName = std::move(name);
	gameObject->Initialize();

	if (parent)
	{
		gameObject->mParent = parent;
		parent->mChilds.push_back(gameObject);
	}

	// Add game object to the update list
	mUpdateList.push_back(gameObject);

	return handle;
}

GameObjectHandle World::Find(const std::string& name)
{
	using namespace std;
	auto iter = find_if(begin(mUpdateList), end(mUpdateList), [&name](auto gameObject)
	{
		return gameObject->GetName() == name;
	});
	return (iter != end(mUpdateList)) ? (*iter)->GetHandle() : GameObjectHandle();

	//for (auto& gameObject : mUpdateList)
	//{
	//	if (gameObject->GetName() == name)
	//		return gameObject->GetHandle();
	//}
	//return {};
}

void World::Destroy(GameObjectHandle handle)
{
	// If handle is invalid, nothing to do so just exit
	if (!handle.IsValid())
		return;

	// Cache the point er and unregister with the handle pool
	GameObject* gameObject = handle.Get();
	mGameObjectHandlePool->Unregister(handle);

	// Check if we can destroy it now
	if (!mUpdating)
		DestroyInternal(gameObject);
	else
		mDestroyList.push_back(gameObject);
}

void World::Update(float deltaTime)
{
	ASSERT(!mUpdating, "[World] Already updating the world.");

	for (auto& service : mServices)
		service->Update(deltaTime);

	// Lock the update list
	mUpdating = true;

	// Re-compute size in case new objects are added to the update
	// list during iteration.
	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (gameObject->GetHandle().IsValid())
			gameObject->Update(deltaTime);
	}
	
	// unlock the update list
	mUpdating = false;

	// now wecan safely destroy object
	ProcessDestroyList();
}

void World::Render()
{
	for (auto& service : mServices)
		service->Render();
	for (auto gameObject : mUpdateList)
		gameObject->Render();
}

void World::DebugUI()
{
	for (auto& service : mServices)
		service->DebugUI();
	for (auto gameObject : mUpdateList)
		gameObject->DebugUI();
}

void NFGE::World::DestroyInternal(GameObject * gameObject)
{
	ASSERT(!mUpdating, "[World] Cannot destroy game objects during update.");

	// If pointer is invalidm nothing to do so just exit
	if (gameObject == nullptr)
		return;

	// First remove it from the update list
	auto iter = std::find(mUpdateList.begin(), mUpdateList.end(), gameObject);
	if (iter != mUpdateList.end())
		mUpdateList.erase(iter);

	// Terminate the game object
	gameObject->Terminate();

	// Next destroy the game object
	mGameObjectFactory->Destory(gameObject);
}

void NFGE::World::ProcessDestroyList()
{
	for (auto gameObject : mDestroyList)
		DestroyInternal(gameObject);
	mDestroyList.clear();
}

