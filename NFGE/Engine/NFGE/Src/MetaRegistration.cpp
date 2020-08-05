#include "Precompiled.h"
#include "MetaRegistration.h"

#include "GameObject.h"
#include "Component.h"

// Components
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"

// Services
#include "CameraService.h"
#include "TerrainService.h"

void NFGE::StaticMetaRegister()
{
	META_REGISTER(NFGE::GameObject);
	META_REGISTER(NFGE::Component);

	META_REGISTER(NFGE::ColliderComponent);
	META_REGISTER(NFGE::TransformComponent);
	META_REGISTER(NFGE::MeshRenderComponent);

	META_REGISTER(NFGE::CameraService);
	META_REGISTER(NFGE::TerrainService);

	//META_REGISTER(NFGE::CameraEntry);
}