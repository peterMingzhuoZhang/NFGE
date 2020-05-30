#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"

using namespace NFGE;

namespace
{
	
}

NFGE::GameObjectFactory::GameObjectFactory(GameObjectAllocator & allocator)
	: mGameObjectAllocator(allocator)
{
}

GameObject * NFGE::GameObjectFactory::Create(const char * templateFileName)
{
	using namespace rapidjson;

	GameObject* gameObject = mGameObjectAllocator.New();
	if (gameObject != nullptr)
	{
		FILE* file = nullptr;
		fopen_s(&file, templateFileName, "r");

		char readBuffer[65536];
		FileReadStream is(file, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

		if (document.HasMember("GameObject") && document["GameObject"].IsObject())
		{
			auto jsonObject = document["GameObject"].GetObjectW();
			if (jsonObject.HasMember("Components") && jsonObject["Components"].IsObject())
			{
				auto components = jsonObject["Components"].GetObjectW();
				for (auto& component : components)
				{
					auto metaClass = Core::Meta::FindMetaClass(component.name.GetString());
					auto newComponent = gameObject->AddComponent(metaClass);
					ASSERT(newComponent, "[GameObjectFactory] Failed to create component %s.", component.name.GetString());
					metaClass->Deserialize(newComponent,component.value);
				}
			}
		}
		
		fclose(file);
	}
	return gameObject;
}

void NFGE::GameObjectFactory::Destory(GameObject * gameObject)
{
	mGameObjectAllocator.Delete(gameObject);
}
