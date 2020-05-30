#include "Precompiled.h"
#include "BuildInGeometryMesh.h"
#include "MeshBuilder.h"


namespace
{
	//Plane
	const char* DefaultPlane_Normal = "Wood_012_Tiles_normal.jpg";
}

void NFGE::Graphics::Geometry::PlaneMesh::DefaultInitialize(NFGE::Graphics::DirectionalLight * light)
{
	Load(100.0f, 100.0f, 10.0f, 100.0f, 100.0f, light, nullptr, DefaultPlane_Normal, nullptr, nullptr);
}

void NFGE::Graphics::Geometry::PlaneMesh::Load(int row, int col, float unitLength, float widthScale, float lengthScale, DirectionalLight * light, const char * diffuse, const char * normal, const char * speclur, const char * displacement)
{
	mMesh = NFGE::Graphics::MeshBuilder::CreatePlane(row, col, unitLength);
	NFGE::Graphics::GeometryMesh::Load(light);

	mContext.custumAdjustMatrix = NFGE::Math::Matrix4::sIdentity() * NFGE::Math::Matrix4::sScaling({ widthScale, lengthScale, 1.0f }) * NFGE::Math::Matrix4::sRotationX(NFGE::Math::Constants::Pi * 0.5f);

	if (diffuse) mContext.AddTexture(MeshTextureMaterial::DIFFUSE, TextureManager::Get()->LoadTexture(diffuse));
	if (normal) mContext.AddTexture(MeshTextureMaterial::NORMALS, TextureManager::Get()->LoadTexture(normal));
	if (speclur) mContext.AddTexture(MeshTextureMaterial::SPECULAR, TextureManager::Get()->LoadTexture(speclur));
	if (diffuse) mContext.AddTexture(MeshTextureMaterial::DISPLACEMENT, TextureManager::Get()->LoadTexture(displacement));

	uint32_t contextIndex = mContext.contextIndexTracker[PostProcessContext::Type_SHADOW_02];
	auto postProcessShadowContext = dynamic_cast<PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow*>(mContext.effectedPostProcessContext[contextIndex].get());
	postProcessShadowContext->isCastShadow = false;
}

void NFGE::Graphics::Geometry::SphereMesh::DefaultInitialize(NFGE::Graphics::DirectionalLight* light)
{
	Load(8, 8, 0.5f, light);
}

void NFGE::Graphics::Geometry::SphereMesh::Load(int HeightFactor, int roundFactor, float radius, DirectionalLight * light)
{
	mMesh = NFGE::Graphics::MeshBuilder::CreateSphere(HeightFactor, roundFactor, radius);
	NFGE::Graphics::GeometryMesh::Load(light);
}

void NFGE::Graphics::Geometry::ConeMesh::DefaultInitialize(NFGE::Graphics::DirectionalLight* light)
{
	Load(8, 16, 0.5f, 1.0f, light);
}

void NFGE::Graphics::Geometry::ConeMesh::Load(int row, int col, float radius, float height, DirectionalLight * light)
{
	mMesh = NFGE::Graphics::MeshBuilder::CreateCone(row, col, radius, height);
	NFGE::Graphics::GeometryMesh::Load(light);
}

