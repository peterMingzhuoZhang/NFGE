#include "Precompiled.h"
#include "Effect.h"
#include "EffectManager.h"
#include "RasterizerStateManager.h"

#include "Camera.h"

using namespace NFGE::Graphics;
using namespace NFGE::Graphics::Effects;

// -------------------------------------------------------------------------------------------
// --- OnScreenEffect ------------------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---

void NFGE::Graphics::Effects::OnScreenEffect::Initialize()
{
	mVertexShader.Initialize( (EffectManager::Get()->GetRootPath() / "Effects/Effect_OnScreen.fx").wstring().c_str(), VertexPC::Format);
	mPixelShader.Initialize((EffectManager::Get()->GetRootPath() /"Effects/Effect_OnScreen.fx").wstring().c_str());
}

void NFGE::Graphics::Effects::OnScreenEffect::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void NFGE::Graphics::Effects::OnScreenEffect::AfterUseReset()
{

	mContext = nullptr;
}

void NFGE::Graphics::Effects::OnScreenEffect::Set(EffectContext* contextPtr)
{
	mContext = dynamic_cast<OnScreenEffectContext*>(contextPtr);
	ASSERT(mContext, "[OnScreenEffect] EffectContext type does not match");
}

void NFGE::Graphics::Effects::OnScreenEffect::Bind(const Camera& camera)
{
	ASSERT(mContext, "[OnScreenEffect] No context set up before Bind to video card");
	// Check the context ptr not equal nullptr, but this effect do not need context. So it is ok not doing it.
	mVertexShader.Bind();
	mPixelShader.Bind();
	
	RasterizerStateManager::Get()->GetRasterizerState(mContext->rasterizerState)->Set();

	AfterUseReset();
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- OnScreenEffect ------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------
// --- ColorMeshEffect -----------------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---
void NFGE::Graphics::Effects::ColorMeshEffect::Initialize()
{
	mVertexShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_ColorMesh.fx").wstring().c_str(), VertexPC::Format);
	mPixelShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_ColorMesh.fx").wstring().c_str());
	mConstantBuffer.Initialize(3 * sizeof(NFGE::Math::Matrix4));
}

void NFGE::Graphics::Effects::ColorMeshEffect::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mConstantBuffer.Terminate();
}

void NFGE::Graphics::Effects::ColorMeshEffect::Set(EffectContext * contextPtr)
{
	mContext = dynamic_cast<ColorMeshEffectContext*>(contextPtr);
	ASSERT(mContext, "[OnScreenEffect] EffectContext type does not match");
}

void NFGE::Graphics::Effects::ColorMeshEffect::Bind(const Camera& camera)
{
	using namespace NFGE::Math;
	ASSERT(mContext, "[OnScreenEffect] No context set up before Bind to video card");
	// Check the context ptr not equal nullptr, but this effect do not need context. So it is ok not doing it.
	mVertexShader.Bind();
	mPixelShader.Bind();

	Matrix4 matrices[3];
	matrices[0] = Matrix4::sScaling(mContext->scale) * MatrixRotationQuaternion(mContext->rotation) * Matrix4::sTranslation(mContext->position);
	matrices[1] = camera.GetViewMatrix();
	matrices[2] = camera.GetPerspectiveMatrix();
	auto& transformData = mContext->mTransformData;
	transformData.world = Transpose(matrices[0]);
	transformData.wvp = Transpose(matrices[0] * matrices[1] * matrices[2]);
	transformData.viewPositon = camera.GetPosition();
	matrices[0] = Transpose(matrices[0]);
	matrices[1] = Transpose(matrices[1]);
	matrices[2] = Transpose(matrices[2]);
	mConstantBuffer.Load(matrices);
	mConstantBuffer.BindVS();

	RasterizerStateManager::Get()->GetRasterizerState(mContext->rasterizerState)->Set();

	AfterUseReset();
}

void NFGE::Graphics::Effects::ColorMeshEffect::AfterUseReset()
{
	mContext = nullptr;
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- ColorMeshEffect -----------------------------------------------------------------------
// -------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------
// --- TextureMeshEffect ---------------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---

void NFGE::Graphics::Effects::TextureMeshEffect::Initialize()
{
	mVertexShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_TextureMesh.fx").wstring().c_str(), VertexPX::Format);
	mPixelShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_TextureMesh.fx").wstring().c_str());

	mConstantBuffer.Initialize(3 * sizeof(NFGE::Math::Matrix4));
	sampler = SamplerManager::Get()->GetSampler("PointWrap");
}

void NFGE::Graphics::Effects::TextureMeshEffect::Terminate()
{
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void NFGE::Graphics::Effects::TextureMeshEffect::Set(EffectContext * contextPtr)
{
	mContext = dynamic_cast<TextureMeshEffectContext*>(contextPtr);
	ASSERT(mContext, "[OnScreenEffect] EffectContext type does not match");
}

void NFGE::Graphics::Effects::TextureMeshEffect::Bind(const Camera& camera)
{
	using namespace NFGE::Math;
	ASSERT(mContext, "[OnScreenEffect] No context set up before Bind to video card");
	// Check the context ptr not equal nullptr, but this effect do not need context. So it is ok not doing it.
	mVertexShader.Bind();
	mPixelShader.Bind();

	Matrix4 matrices[3];
	matrices[0] = Matrix4::sScaling(mContext->scale) * MatrixRotationQuaternion(mContext->rotation) * Matrix4::sTranslation(mContext->position);
	matrices[1] = camera.GetViewMatrix();
	matrices[2] = camera.GetPerspectiveMatrix();
	auto& transformData = mContext->mTransformData;
	transformData.world = Transpose(matrices[0]);
	transformData.wvp = Transpose(matrices[0] * matrices[1] * matrices[2]);
	transformData.viewPositon = camera.GetPosition();
	matrices[0] = Transpose(matrices[0]);
	matrices[1] = Transpose(matrices[1]);
	matrices[2] = Transpose(matrices[2]);
	mConstantBuffer.Load(matrices);
	mConstantBuffer.BindVS();

	TextureManager::Get()->GetTexture(mContext->textureId)->BindPS();
	sampler->BindPS();
	
	RasterizerStateManager::Get()->GetRasterizerState(mContext->rasterizerState)->Set();

	AfterUseReset();
}

void NFGE::Graphics::Effects::TextureMeshEffect::AfterUseReset()
{
	mContext = nullptr;
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- TextureMeshEffect ---------------------------------------------------------------------
// -------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------
// --- StandardMeshEffect --------------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---

void StandardMeshEffect::Initialize()
{
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionDateBuffer.Initialize();
	mShadowConstantBuffer.Initialize();

	mVertexShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_StandardMesh.fx").wstring().c_str(), Vertex::Format);
	mPixelShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_StandardMesh.fx").wstring().c_str());
	sampler = SamplerManager::Get()->GetSampler("PointWrap");

	//mAffectPostProcessUnit.emplace_back(PostProcessManager::Get()->GetPostProcessUnit(PostProcessType::None)); deprecated
	
}

void StandardMeshEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mShadowConstantBuffer.Terminate();
	mOptionDateBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void StandardMeshEffect::Set(EffectContext* contextPtr)
{
	mContext = dynamic_cast<StandardMeshEffectContext*>(contextPtr);
	ASSERT(mContext, "[StandardMeshEffect] EffectContext type does not match");
}
void StandardMeshEffect::Bind(const Camera& camera)
{
	using namespace NFGE::Math;
	mVertexShader.Bind();
	mPixelShader.Bind();

	Matrix4 AdjustMatrix =	Matrix4::sScaling(mContext->AdjustScale) * 
							Matrix4::sRotationX(DEG2RAD(mContext->AdjustRotation.x)) *
							Matrix4::sRotationY(DEG2RAD(mContext->AdjustRotation.y)) *
							Matrix4::sRotationZ(DEG2RAD(mContext->AdjustRotation.z)) *
							Matrix4::sTranslation(mContext->AdjustPosition);

	Matrix4 matrices[3];
	matrices[0] = (mContext->custumAdjustMatrix * AdjustMatrix * mContext->custumToWorldMatrix * Matrix4::sScaling(mContext->scale) * MatrixRotationQuaternion(mContext->rotation) * Matrix4::sTranslation(mContext->position));
	matrices[1] = (camera.GetViewMatrix());
	matrices[2] = (camera.GetPerspectiveMatrix());
	auto& transformData = mContext->mTransformData;
	transformData.world = Transpose(matrices[0]);
	transformData.wvp = Transpose(matrices[0] * matrices[1] * matrices[2]);
	transformData.viewPositon = camera.GetPosition();
	mTransformBuffer.Load(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	// For shadow
	uint32_t contextIndex = mContext->contextIndexTracker[PostProcessContext::Type_SHADOW_02];
	auto postProcessShadowContext = dynamic_cast<PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow*>(mContext->effectedPostProcessContext[contextIndex].get());
	postProcessShadowContext->toWorldMatrix = matrices[0];

	mLightBuffer.Load(*(mContext->light));
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Load(mContext->material);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	auto postProcessManager = PostProcessManager::Get();
	auto postProcessUnit_shadow = postProcessManager->GetPostProcessUnit<PostProcesses::PostProcessUnit_Shadow>();
	OptionsData optData;
	optData.displacementWeight = mContext->bumpWeight;
	optData.meshTextureInfoMask = mContext->currentContainTexture & mContext->usingTextureSwitch; // Turn certain texture off base on [usingTextureSwitch];
	optData.useShadow = postProcessManager->GetIsUsingShadow();
	optData.depthBias = postProcessUnit_shadow->GetDepthBias();
	mOptionDateBuffer.Load(optData);
	mOptionDateBuffer.BindVS(3);
	mOptionDateBuffer.BindPS(3);

	// For shadow
	mShadowConstantBuffer.Load(postProcessShadowContext->readyToUseLightWvp);
	mShadowConstantBuffer.BindVS(4);

	// For shadow
	postProcessUnit_shadow->GetRenderTarget()->BindPS(20);

	for (size_t i = 0; i < mContext->textures.size(); ++i)
	{
		switch (mContext->textureTypes[i])
		{
		case NFGE::Graphics::MeshTextureMaterial::DIFFUSE:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::DIFFUSE));
			break;
		case NFGE::Graphics::MeshTextureMaterial::SPECULAR:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::SPECULAR));
			break;
		case NFGE::Graphics::MeshTextureMaterial::AMBIENT:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::AMBIENT));
			break;
		case NFGE::Graphics::MeshTextureMaterial::EMISSIVE:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::EMISSIVE));
			break;
		case NFGE::Graphics::MeshTextureMaterial::HEIGHT:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::HEIGHT));
			break;
		case NFGE::Graphics::MeshTextureMaterial::NORMALS:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::NORMALS));
			break;
		case NFGE::Graphics::MeshTextureMaterial::SHININESS:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::SHININESS));
			break;
		case NFGE::Graphics::MeshTextureMaterial::OPACITY:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::OPACITY));
			break;
		case NFGE::Graphics::MeshTextureMaterial::DISPLACEMENT:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindVS(static_cast<uint32_t>(MeshTextureMaterial::DISPLACEMENT));
			break;
		case NFGE::Graphics::MeshTextureMaterial::LIGHTMAP:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::LIGHTMAP));
			break;
		case NFGE::Graphics::MeshTextureMaterial::REFLECTION:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::REFLECTION));
			break;
		case NFGE::Graphics::MeshTextureMaterial::BASE_COLOR:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::BASE_COLOR));
			break;
		case NFGE::Graphics::MeshTextureMaterial::NORMAL_CAMERA:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::NORMAL_CAMERA));
			break;
		case NFGE::Graphics::MeshTextureMaterial::EMISSION_COLOR:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::EMISSION_COLOR));
			break;
		case NFGE::Graphics::MeshTextureMaterial::MATALNESS:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::MATALNESS));
			break;
		case NFGE::Graphics::MeshTextureMaterial::DIFFUSE_ROUGHNESS:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::DIFFUSE_ROUGHNESS));
			break;
		case NFGE::Graphics::MeshTextureMaterial::AMBIENT_OCCLUSION:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::AMBIENT_OCCLUSION));
			break;
		case NFGE::Graphics::MeshTextureMaterial::UNKNOWN:
			TextureManager::Get()->GetTexture(mContext->textures[i])->BindPS(static_cast<uint32_t>(MeshTextureMaterial::UNKNOWN));
			break;
		default:
			break;
		}
	}

	RasterizerStateManager::Get()->GetRasterizerState(mContext->rasterizerState)->Set();

	AfterUseReset();
}
void StandardMeshEffect::AfterUseReset() 
{
	mContext = nullptr;
}
// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- StandardMeshEffect --------------------------------------------------------------------
// -------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------
// --- ModelMeshEffect -----------------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---
void NFGE::Graphics::Effects::ModelMeshEffect::Initialize()
{
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionDateBuffer.Initialize();
	mShadowConstantBuffer.Initialize();

	mVertexShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_ModelMesh.fx").wstring().c_str(), Vertex::Format);
	mPixelShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_ModelMesh.fx").wstring().c_str());
	sampler = SamplerManager::Get()->GetSampler("PointWrap");
}

void NFGE::Graphics::Effects::ModelMeshEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mShadowConstantBuffer.Terminate();
	mOptionDateBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void NFGE::Graphics::Effects::ModelMeshEffect::Set(EffectContext * contextPtr)
{
	mContext = dynamic_cast<ModelMeshEffectContext*>(contextPtr);
	ASSERT(mContext, "[StandardMeshEffect] EffectContext type does not match");
	if (mLastContextPtr == mContext)
	{
		++mMeshIndex;
		mIsNeedFullBind = false;
	}
	else
	{
		mMeshIndex = 0;
		mIsNeedFullBind = true;
	}
}

void NFGE::Graphics::Effects::ModelMeshEffect::Bind(const Camera& camera)
{
	using namespace NFGE::Math;

	// For shadow
	uint32_t contextIndex = mContext->contextIndexTracker[PostProcessContext::Type_SHADOW_02];
	auto postProcessShadowContext = dynamic_cast<PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow*>(mContext->effectedPostProcessContext[contextIndex].get());

	if (mIsNeedFullBind)
	{
		mVertexShader.Bind();
		mPixelShader.Bind();

		Matrix4 AdjustMatrix = Matrix4::sScaling(mContext->AdjustScale) *
			Matrix4::sRotationX(DEG2RAD(mContext->AdjustRotation.x)) *
			Matrix4::sRotationY(DEG2RAD(mContext->AdjustRotation.y)) *
			Matrix4::sRotationZ(DEG2RAD(mContext->AdjustRotation.z)) *
			Matrix4::sTranslation(mContext->AdjustPosition);

		Matrix4 matrices[3];
		matrices[0] = (mContext->custumAdjustMatrix * AdjustMatrix * mContext->custumToWorldMatrix * Matrix4::sScaling(mContext->scale) * NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(mContext->currentFoward, Vector3::YAxis)) * MatrixRotationQuaternion(mContext->rotation) * Matrix4::sTranslation(mContext->position));
		matrices[1] = (camera.GetViewMatrix());
		matrices[2] = (camera.GetPerspectiveMatrix());
		auto& transformData = mContext->mTransformData;
		transformData.world = Transpose(matrices[0]);
		transformData.wvp = Transpose(matrices[0] * matrices[1] * matrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);
		mTransformBuffer.BindPS(0);
		
		// For shadow
		postProcessShadowContext->toWorldMatrix = matrices[0];

		mLightBuffer.Load(*(mContext->light));
		mLightBuffer.BindVS(1);
		mLightBuffer.BindPS(1);
	}

	mMaterialBuffer.Load(mContext->materials[mContext->materialIndices[mMeshIndex]]);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);
	
	using TextureType = MeshTextureMaterial::ModelTextureType;
	auto& currentModelMaterial = mContext->modelMaterials[mContext->materialIndices[mMeshIndex]];
	uint32_t currentContainTexture = currentModelMaterial.mAvliableTextureType;

	if (currentContainTexture & MeshTextureMaterial::TextureType_DIFFUSE)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DIFFUSE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::DIFFUSE));
	if (currentContainTexture & MeshTextureMaterial::TextureType_SPECULAR)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SPECULAR].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SPECULAR));
	if (currentContainTexture & MeshTextureMaterial::TextureType_AMBIENT)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::AMBIENT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::AMBIENT));
	if (currentContainTexture & MeshTextureMaterial::TextureType_EMISSIVE)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::EMISSIVE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::EMISSIVE));
	if (currentContainTexture & MeshTextureMaterial::TextureType_HEIGHT)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::HEIGHT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::HEIGHT));
	if (currentContainTexture & MeshTextureMaterial::TextureType_NORMALS)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::NORMALS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::NORMALS));
	if (currentContainTexture & MeshTextureMaterial::TextureType_SHININESS)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SHININESS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SHININESS));
	if (currentContainTexture & MeshTextureMaterial::TextureType_OPACITY)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::OPACITY].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::OPACITY));
	if (currentContainTexture & MeshTextureMaterial::TextureType_DISPLACEMENT)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DISPLACEMENT].mTextureIds[0])->BindVS(static_cast<uint32_t>(TextureType::DISPLACEMENT));
	//TODO:: Bind more if needed

	// For shadow
	auto postProcessManager = PostProcessManager::Get();
	auto postProcessUnit_shadow = postProcessManager->GetPostProcessUnit<PostProcesses::PostProcessUnit_Shadow>();

	OptionsData optData;
	optData.displacementWeight = mContext->bumpWeight;
	optData.meshTextureInfoMask = currentContainTexture;
	optData.useShadow = postProcessManager->GetIsUsingShadow();		//|
	optData.depthBias = postProcessUnit_shadow->GetDepthBias();		//|--- For shadow
	mOptionDateBuffer.Load(optData);
	mOptionDateBuffer.BindVS(3);
	mOptionDateBuffer.BindPS(3);

	// For shadow
	mShadowConstantBuffer.Load(postProcessShadowContext->readyToUseLightWvp);
	mShadowConstantBuffer.BindVS(4);

	// For shadow
	postProcessUnit_shadow->GetRenderTarget()->BindPS(20);

	RasterizerStateManager::Get()->GetRasterizerState(mContext->rasterizerState)->Set();

	AfterUseReset();
}

void NFGE::Graphics::Effects::ModelMeshEffect::AfterUseReset()
{
	mLastContextPtr = mContext;
	if (mMeshIndex == mContext->materialIndices.size() - 1) // Reset mLastContextPtr to nullptr when current object finish drawing
		mLastContextPtr = nullptr;

	mContext = nullptr;
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- ModelMeshEffect -----------------------------------------------------------------------
//  ------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------
// --- SkeletonModelMeshEffect ---------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---

void SkeletonModelMeshEffect::SkeletonModelMeshEffectContext::UpdateTransform(int index)
{
	auto& currentToParent = mSkeleton.bones[index]->toParentTransform;
	//currentToParent = //Math::MatrixRotationQuaternion(mDynamicToParentTransform.mCurrentRotation) * Matrix4::sTranslation(mDynamicToParentTransform.mCurrentTranslate);
	UpdateTransformImpl(mSkeleton.bones[index].get(), boneMatrix);
}

void SkeletonModelMeshEffect::SkeletonModelMeshEffectContext::UpdateTransformImpl(Bone* bone, std::vector<NFGE::Math::Matrix4>& boneMatri)
{
	if (bone->parent)
		boneMatrix[bone->index] = bone->toParentTransform * boneMatrix[bone->parentIndex];
	else
		boneMatrix[bone->index] = bone->toParentTransform;
	for (auto child : bone->children)
		UpdateTransformImpl(child, boneMatrix);
}

void SkeletonModelMeshEffect::Initialize()
{
	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionDateBuffer.Initialize();
	mShadowConstantBuffer.Initialize();

	mVertexShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_SkeletonModelMesh.fx").wstring().c_str(), BoneVertex::Format);
	mPixelShader.Initialize((EffectManager::Get()->GetRootPath() / "Effects/Effect_SkeletonModelMesh.fx").wstring().c_str());
	sampler = SamplerManager::Get()->GetSampler("LinearWrap");

}
void SkeletonModelMeshEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mShadowConstantBuffer.Terminate();
	mOptionDateBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
}
void SkeletonModelMeshEffect::Set(EffectContext* contextPtr)
{
	mContext = dynamic_cast<SkeletonModelMeshEffectContext*>(contextPtr);
	ASSERT(mContext, "[StandardMeshEffect] EffectContext type does not match");
	if (mLastContextPtr == mContext)
	{
		++mMeshIndex;
		mIsNeedFullBind = false;
	}
	else
	{
		mMeshIndex = 0;
		mIsNeedFullBind = true;
	}
}
void SkeletonModelMeshEffect::Bind(const Camera& camera)
{
	using namespace NFGE::Math;

	// For shadow
	uint32_t contextIndex = mContext->contextIndexTracker[PostProcessContext::Type_SHADOW_02];
	auto postProcessShadowContext = dynamic_cast<PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow*>(mContext->effectedPostProcessContext[contextIndex].get());

	if (mIsNeedFullBind)
	{
		mVertexShader.Bind();
		mPixelShader.Bind();

		mContext->finalAdjustMatrix = 
			mContext->custumAdjustMatrix *
			Matrix4::sScaling(mContext->AdjustScale) *
			Matrix4::sRotationX(DEG2RAD(mContext->AdjustRotation.x)) *
			Matrix4::sRotationY(DEG2RAD(mContext->AdjustRotation.y)) *
			Matrix4::sRotationZ(DEG2RAD(mContext->AdjustRotation.z)) *
			Matrix4::sTranslation(mContext->AdjustPosition);

		mContext->finalToWorld =
			mContext->custumToWorldMatrix *
			Matrix4::sScaling(mContext->scale) *
			NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionLookRotation(mContext->currentFoward, Vector3::YAxis)) *
			MatrixRotationQuaternion(mContext->rotation) *
			Matrix4::sTranslation(mContext->position);

		Matrix4 matrices[3];
		matrices[0] = (mContext->finalAdjustMatrix * mContext->finalToWorld);
		matrices[1] = (camera.GetViewMatrix());
		matrices[2] = (camera.GetPerspectiveMatrix());
		auto& transformData = mContext->mTransformData;
		transformData.world = Transpose(matrices[0]);
		transformData.wvp = Transpose(matrices[0] * matrices[1] * matrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);
		mTransformBuffer.BindPS(0);

		// For shadow
		postProcessShadowContext->toWorldMatrix = matrices[0];

		mLightBuffer.Load(*(mContext->light));
		mLightBuffer.BindVS(1);
		mLightBuffer.BindPS(1);

		auto& bones = mContext->mSkeleton.bones;
		for (int i = 0; i < bones.size(); i++)
		{
			mContext->mBoneTransformMatrix[i] = Transpose(bones[i]->offsetTransform * mContext->boneMatrix[i]);
		}
		mBoneTransformBuffer.Load(mContext->mBoneTransformMatrix);
		mBoneTransformBuffer.BindVS(4);
	}
	
	mMaterialBuffer.Load(mContext->materials[mContext->materialIndices[mMeshIndex]]);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	using TextureType = MeshTextureMaterial::ModelTextureType;
	auto& currentModelMaterial = mContext->modelMaterials[mContext->materialIndices[mMeshIndex]];
	uint32_t currentContainTexture = currentModelMaterial.mAvliableTextureType;

	if (currentContainTexture & MeshTextureMaterial::TextureType_DIFFUSE)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DIFFUSE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::DIFFUSE));
	if (currentContainTexture & MeshTextureMaterial::TextureType_SPECULAR)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SPECULAR].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SPECULAR));
	if (currentContainTexture & MeshTextureMaterial::TextureType_AMBIENT)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::AMBIENT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::AMBIENT));
	if (currentContainTexture & MeshTextureMaterial::TextureType_EMISSIVE)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::EMISSIVE].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::EMISSIVE));
	if (currentContainTexture & MeshTextureMaterial::TextureType_HEIGHT)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::HEIGHT].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::HEIGHT));
	if (currentContainTexture & MeshTextureMaterial::TextureType_NORMALS)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::NORMALS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::NORMALS));
	if (currentContainTexture & MeshTextureMaterial::TextureType_SHININESS)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::SHININESS].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::SHININESS));
	if (currentContainTexture & MeshTextureMaterial::TextureType_OPACITY)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::OPACITY].mTextureIds[0])->BindPS(static_cast<uint32_t>(TextureType::OPACITY));
	if (currentContainTexture & MeshTextureMaterial::TextureType_DISPLACEMENT)
		TextureManager::Get()->GetTexture(currentModelMaterial.mTextureHolders[TextureType::DISPLACEMENT].mTextureIds[0])->BindVS(static_cast<uint32_t>(TextureType::DISPLACEMENT));
	//TODO:: Bind more if needed

	// For shadow
	auto postProcessManager = PostProcessManager::Get();
	auto postProcessUnit_shadow = postProcessManager->GetPostProcessUnit<PostProcesses::PostProcessUnit_Shadow>();

	OptionsData optData;
	optData.displacementWeight = mContext->bumpWeight;
	optData.meshTextureInfoMask = currentContainTexture;
	optData.useShadow = postProcessManager->GetIsUsingShadow();	//|
	optData.depthBias = postProcessUnit_shadow->GetDepthBias();	//|--- For shadow
	mOptionDateBuffer.Load(optData);
	mOptionDateBuffer.BindVS(3);
	mOptionDateBuffer.BindPS(3);

	// For shadow
	mShadowConstantBuffer.Load(postProcessShadowContext->readyToUseLightWvp);
	mShadowConstantBuffer.BindVS(5);

	// For shadow
	postProcessUnit_shadow->GetRenderTarget()->BindPS(20);

	RasterizerStateManager::Get()->GetRasterizerState(mContext->rasterizerState)->Set();

	AfterUseReset();
}
void SkeletonModelMeshEffect::AfterUseReset()
{
	mLastContextPtr = mContext;
	if (mMeshIndex == mContext->materialIndices.size() - 1) // Reset mLastContextPtr to nullptr when current object finish drawing
		mLastContextPtr = nullptr;

	mContext = nullptr;
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- SkeletonModelMeshEffect ---------------------------------------------------------------
//  ------------------------------------------------------------------------------------------