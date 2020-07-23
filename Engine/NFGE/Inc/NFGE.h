//====================================================================================================
// Filename:	NFGE.h
// Created by:	Mingzhuo Zhang
//====================================================================================================

#ifndef INCLIDED_NFGE_H
#define INCLIDED_NFGE_H

#include "Common.h"

// App headers
#include "App.h"
#include "AppState.h"

// Components headers
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "MeshRenderComponent.h"

// World headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Service.h"
#include "World.h"

// Editor
#include "Editor.h"

#include "QuadTree.h"
#include "Terrain.h"
#include "TerrainService.h"
#include "CameraService.h"

#include "MetaRegistration.h"

namespace NFGE { extern App sApp; }

namespace NFGEApp
{

	template<class T>
	void AddState(std::string name)
	{
		NFGE::sApp.AddState<T>(name);
	}	

	void ChangeState(std::string name);
	void Run(NFGE::AppConfig appConfig);
	void ShutDown();

}
#endif // !INCLIDED_NFGE_H
