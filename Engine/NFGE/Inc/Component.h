#ifndef INCLIDED_NFGE_COMPONENT_H
#define	INCLIDED_NFGE_COMPONENT_H

namespace NFGE
{
	class GameObject;
	class Component
	{
	public:
		META_CLASS_DECLARE;

		virtual ~Component() = default;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner() const { return *mOwner; }

	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}

#endif // !INCLIDED_NFGE_COMPONENT_H
