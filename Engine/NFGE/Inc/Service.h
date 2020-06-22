#ifndef INCLIDED_NFGE_SERVICE_H
#define	INCLIDED_NFGE_SERVICE_H

namespace NFGE
{
	class World;

	class Service
	{
	public:
		META_CLASS_DECLARE;

		Service() = default;
		virtual ~Service() = default;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void WorldViewUI() {}
		virtual void InspectorUI(void (*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*) ) {}
		virtual void DebugUI() {}

		World& GetWorld() { return *mWorld; }
		const World& GetWorld() const { return *mWorld; }

	private:
		friend class World;
		World* mWorld = nullptr;
	};
}

#endif // !INCLIDED_NFGE_GAMEOBJECT_H
