#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

namespace
{
	const uint32_t InitChildrenSize = 5;
}

using namespace NFGE;

META_CLASS_BEGIN(GameObject)
	META_FIELD_BEGIN
	META_FIELD(mName, "name")
	META_FIELD_END
META_CLASS_END

void GameObject::Initialize()
{
	mChilds.reserve(InitChildrenSize);

	for (auto& component : mComponents)
		component->Initialize();
}

void GameObject::Terminate()
{
	for (auto& component : mComponents)
		component->Terminate();
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
		component->Update(deltaTime);
}

void GameObject::Render()
{
	for (auto& component : mComponents)
		component->Render();
}

void NFGE::GameObject::InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass *, uint8_t *))
{
	ShowMetaClassInInspector(GetMetaClass(), (uint8_t*)this);
	for (auto& component : mComponents)
	{
		component->InspectorUI(ShowMetaClassInInspector);
	}
}

void GameObject::DebugUI()
{
	for (auto& component : mComponents)
		component->DebugUI();
}

Component * NFGE::GameObject::AddComponent(const NFGE::Core::Meta::MetaClass * metaClass)
{
	Component* newComponent = static_cast<Component*>(metaClass->Create());
	newComponent->mOwner = this;
	mComponents.emplace_back(std::unique_ptr<Component>(newComponent));
	return newComponent;
}

void NFGE::GameObject::RemoveChild(GameObject * child)
{
	if (mChilds.empty())
		return;
	auto iter = std::find(mChilds.begin(), mChilds.end(), child);
	if (iter != mChilds.end())
	{
		std::iter_swap(iter, mChilds.end() - 1);
		mChilds.pop_back();
	}
}

void NFGE::GameObject::AddChild(GameObject * child)
{
	auto childOldParent = child->GetParent();
	if (childOldParent == this)
		return;
	if (childOldParent != nullptr)
		childOldParent->RemoveChild(child);

	mChilds.push_back(child);
	child->mParent = this;
}
