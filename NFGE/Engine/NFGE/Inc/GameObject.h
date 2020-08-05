#ifndef INCLIDED_NFGE_GAMEOBJECT_H
#define	INCLIDED_NFGE_GAMEOBJECT_H

namespace NFGE
{
	class Component;
	class GameObject;
	class World;

	using GameObjectAllocator = Core::TypeAllocator<GameObject>;
	using GameObjectHandle = Core::Handle<GameObject>;
	using GameObjectHandlePool = Core::HandlePool<GameObject>;

	class GameObject final
	{
	public:
		META_CLASS_DECLARE

		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*));
		void DebugUI();
		
		Component* AddComponent(const NFGE::Core::Meta::MetaClass* metaClass);

		template<class T, class = std::void_t<std::is_base_of<Component, T>>>
		T* AddComponent()
		{
			auto& newComponent = mComponents.emplace_back(std::make_unique<T>());
			newComponent->mOwner = this;
			return static_cast<T*>(newComponent.get());
		}

		template<class ComponentType>
		const ComponentType* GetComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetMetaClass() == ComponentType::StaticGetMetaClass())
					return static_cast<ComponentType*>(component.get());
			}
			return nullptr;
		}

		template<class ComponentType>
		ComponentType* GetComponent()
		{
			const GameObject* constMe = this;
			return const_cast<ComponentType*>(constMe->GetComponent<ComponentType>());
		}

		const World& GetWorld() const { return *mWorld; }
		World& GetWorld() { return *mWorld; }

		void SetName(const char* name) { mName = name; }
		std::string& GetName() { return mName; }

		GameObjectHandle GetHandle() const { return mHandle; }

		GameObject* GetParent() { return mParent; }
		std::vector<GameObject*>& GetChildren() { return mChilds; }
		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);
		void ResetParent(GameObject* newParent);
	private:
		friend class Editor;
		friend class World;
		using Components = std::vector<std::unique_ptr<Component>>;

		World* mWorld = nullptr;
		std::string mName = "NoName";
		GameObjectHandle mHandle;
		Components mComponents;

		GameObject* mParent = nullptr;
		std::vector<GameObject*> mChilds;

		bool mIsShowChildInEditor = false;
	};
}

#endif // !INCLIDED_NFGE_GAMEOBJECT_H
