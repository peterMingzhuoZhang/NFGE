#include "Precompiled.h"
#include "PostProcessUnit.h"
#include "PostProcessManager.h"
#include "EffectManager.h"
#include "MeshBuilder.h"

using namespace NFGE::Graphics;

namespace
{
	const wchar_t* PostProcessNone = L"PostProcesses/PostProcess_None.fx";
	const wchar_t* PostProcessPixelate = L"PostProcesses/PostProcess_Pixelate.fx";
	const wchar_t* PostProcessShadow = L"PostProcesses/PostProcess_Shadow.fx";
	const wchar_t* PostProcessDepthBlur_0 = L"PostProcesses/PostProcess_DepthBlue_Pre.fx";
	const wchar_t* PostProcessDepthBlur_1 = L"PostProcesses/PostProcess_DepthBlur_Post.fx";

	const uint32_t ShadowCameraResulotionMultiplier = 4;
	const uint32_t shadowRenderTargetWidth = 1280;
	const uint32_t shadowRenderTargetHeight = 720;
}

void NFGE::Graphics::PostProcessUnit::Initialize()
{
	mEffectedObjects.reserve(EffectManager::InitialRenderObjectCapacity);
}

// -------------------------------------------------------------------------------------------
// --- PostProcessUnit_None ------------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---

PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_None::sGetType()
{
	return PostProcessType::None;
}

PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_None::GetType()
{
	return PostProcessType::None;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_None::Initialize()
{
	PostProcessUnit::Initialize();
	mEffectedObjectContext.reserve(EffectManager::InitialRenderObjectCapacity);
	auto graphicSystem = GraphicsSystem::Get();
	mScreenMesh = MeshBuilder::CreateNDCQuad();
	mScreenMeshBuffer.Initialize(mScreenMesh);
	mRenderTarget.Initialize(graphicSystem->GetBackBufferWidth(), graphicSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);

	mPostProcessPixelShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessNone).wstring().c_str());
	mPostProcessVertexShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessNone).wstring().c_str(), VertexPX::Format);
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_None::Terminate()
{
	mScreenMeshBuffer.Terminate();
	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mRenderTarget.Terminate();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_None::PreProcess()
{
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_None::PostProcess()
{
	auto postProcessManager = PostProcessManager::Get();
	auto lastRenderTargetPtr = postProcessManager->GetLastRenderTargetPtr();
	mRenderTarget.BeginRender();
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();
	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);
	lastRenderTargetPtr->BindPS(0);
	mScreenMeshBuffer.Render();
	lastRenderTargetPtr->UnbindPS(0);
	mRenderTarget.EndRender();
	lastRenderTargetPtr = &mRenderTarget;

}

void NFGE::Graphics::PostProcesses::PostProcessUnit_None::OptionUI()
{
}

std::string NFGE::Graphics::PostProcesses::PostProcessUnit_None::GetName()
{
	return "PostProcessUnit_None";
}

uint32_t NFGE::Graphics::PostProcesses::PostProcessUnit_None::GetContextTypeMask()
{
	return PostProcessUnit_None::ContextTypeMask;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_None::ToScreenRender()
{
	auto postProcessManager = PostProcessManager::Get();
	auto lastRenderTargetPtr = postProcessManager->GetLastRenderTargetPtr();
	
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();
	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);
	lastRenderTargetPtr->BindPS(0);
	mScreenMeshBuffer.Render();
	lastRenderTargetPtr->UnbindPS(0);
	
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- PostProcessUnit_None ------------------------------------------------------------------
// -------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------
// --- PostProcessUnit_Pixelate --------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---

PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::sGetType()
{
	return PostProcessType::Pixelate;
}

PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::GetType()
{
	return PostProcessType::Pixelate;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::Initialize()
{
	PostProcessUnit::Initialize();
	mEffectedObjectContext.reserve(EffectManager::InitialRenderObjectCapacity);
	auto graphicSystem = GraphicsSystem::Get();
	mScreenMesh = MeshBuilder::CreateNDCQuad();
	mScreenMeshBuffer.Initialize(mScreenMesh);
	mRenderTarget.Initialize(graphicSystem->GetBackBufferWidth(), graphicSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);

	mPostProcessPixelShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessPixelate).wstring().c_str());
	mPostProcessVertexShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessPixelate).wstring().c_str(), VertexPX::Format);

	mPixelateOptionBuffer.Initialize();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::Terminate()
{
	mPixelateOptionBuffer.Terminate();
	mScreenMeshBuffer.Terminate();
	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mRenderTarget.Terminate();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::PreProcess()
{
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::PostProcess()
{
	auto postProcessManager = PostProcessManager::Get();
	auto lastRenderTargetPtr = postProcessManager->GetLastRenderTargetPtr();
	mRenderTarget.BeginRender();
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();
	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);
	PixelateOptionData optionData;
	optionData.pixelatePower = mPixelatePower;
	mPixelateOptionBuffer.Load(optionData);
	mPixelateOptionBuffer.BindPS(0);
	lastRenderTargetPtr->BindPS(0);
	mScreenMeshBuffer.Render();
	lastRenderTargetPtr->UnbindPS(0);
	mRenderTarget.EndRender();
	postProcessManager->SetLastRenderTargetPtr(&mRenderTarget);
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::OptionUI()
{
	ImGui::DragFloat("Pixelate Power", &mPixelatePower, 1.0f, 0.0f, NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferWidth());
}

std::string NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::GetName()
{
	return "PostProcessUnit_Pixelate";
}

uint32_t NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::GetContextTypeMask()
{
	return PostProcessUnit_Pixelate::ContextTypeMask;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Pixelate::RegisterAffectObject(RenderContext * renderObject)
{
	if (renderObject->mEffectContext->GetPostProcessContextMask() & PostProcessContext::Type_PIXELATE_01)
	{
		mEffectedObjects.push_back(renderObject);
		auto effectContext = renderObject->mEffectContext;
		PostProcessContext_Pixelate* contextPtr = dynamic_cast<PostProcessContext_Pixelate*>(effectContext->effectedPostProcessContext[effectContext->contextIndexTracker[PostProcessContext::Type_PIXELATE_01]].get());
		ASSERT(contextPtr, "Miss match postProcessContext in passed in effectContext");
		mEffectedObjectContext.push_back(contextPtr);
	}
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- PostProcessUnit_Pixelate --------------------------------------------------------------
// -------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------
// --- PostProcessUnit_Shadow ----------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---
PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::sGetType()
{
	return PostProcessType::Shadow;
}

PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::GetType()
{
	return PostProcessType::Shadow;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::Initialize()
{
	PostProcessUnit::Initialize();
	mEffectedObjectContext.reserve(EffectManager::InitialRenderObjectCapacity);
	auto graphicSystem = GraphicsSystem::Get();
	//mScreenMesh = MeshBuilder::CreateNDCQuad();	//No Need
	//mScreenMeshBuffer.Initialize(mScreenMesh);	//No Need
	mRenderTarget.Initialize(shadowRenderTargetWidth * ShadowCameraResulotionMultiplier, shadowRenderTargetHeight * ShadowCameraResulotionMultiplier, RenderTarget::Format::RGBA_U32);

	mPostProcessVertexShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessShadow).wstring().c_str(), BoneVertex::Format);
	mPostProcessPixelShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessShadow).wstring().c_str());

	mDepthContantBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mDepthMapOptionBuffer.Initialize();

	mLightCamera.SetNearPlane(-10000.0f);
	mLightCamera.SetFarPlane(10000.0f);
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::Terminate()
{
	mBoneTransformBuffer.Terminate();
	mDepthMapOptionBuffer.Terminate();
	mDepthContantBuffer.Terminate();

	//mScreenMeshBuffer.Terminate();
	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mRenderTarget.Terminate();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::Active()
{
	PostProcessManager::Get()->SetIsUsingShadow(true);
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::Disable()
{
	PostProcessManager::Get()->SetIsUsingShadow(false);
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::PreProcess()
{
	auto postProcessManager = PostProcessManager::Get();
	mLightCamera.SetDirection(postProcessManager->GetCurrentUseLight()->direction);

	std::vector<Math::Vector3> mViewFrustumVertices =
	{
		// NearPlane
		{ -1.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }, 
		 
		// FarPlane
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
	};

	auto mainCamera = postProcessManager->GetCurrentUseCamera();
	float oriFarplane = mainCamera->GetFarPlane();
	mainCamera->SetFarPlane(oriFarplane * mShadowCoverRange);

	auto defaultMatView = mainCamera->GetViewMatrix();
	auto defaultMatProj = mainCamera->GetPerspectiveMatrix();
	auto invViewProj = Inverse(defaultMatView * defaultMatProj);
	for (auto& vertex : mViewFrustumVertices)
	{
		vertex = TransformCoord(vertex, invViewProj);
	}

	mainCamera->SetFarPlane(oriFarplane);

	auto lightLook = mLightCamera.GetDirection();
	auto lightSide = Math::Normalize(Math::Cross(Math::Vector3::YAxis, lightLook));
	auto lightUp = Math::Normalize(Math::Cross(lightLook, lightSide));
	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;
	float minZ = FLT_MAX, maxZ = -FLT_MAX;

	for (auto& vertex : mViewFrustumVertices)
	{
		float projectX = Dot(lightSide, vertex);
		minX = Math::Min(minX, projectX);
		maxX = Math::Max(maxX, projectX);
		float projectY = Dot(lightUp, vertex);
		minY = Math::Min(minY, projectY);
		maxY = Math::Max(maxY, projectY);
		float projectZ = Dot(lightLook, vertex);
		minZ = Math::Min(minZ, projectZ);
		maxZ = Math::Max(maxZ, projectZ);
	}

	mLightCamera.SetPosition(
		lightSide * (minX + maxX) * 0.5f +
		lightUp * (minY + maxY) * 0.5f +
		lightLook * (minZ + maxZ) * 0.5f
	);

	mLightViewMat = mLightCamera.GetViewMatrix();
	mLightProjMat = mLightCamera.GetOrthographicMatrix(maxX - minX, maxY - minY);
	
	mRenderTarget.BeginRender();
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();
	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);

	int index = 0;
	for (auto shadowContext : mEffectedObjectContext)
	{
		auto wvp = Math::Transpose(shadowContext->toWorldMatrix * mLightViewMat * mLightProjMat);
		shadowContext->readyToUseLightWvp = wvp;

		if (shadowContext->isCastShadow)
		{
			TextureId displacementTextureId = 0;
			mEffectedObjects[index]->mEffectContext->GetTexture(MeshTextureMaterial::DISPLACEMENT, displacementTextureId);
			Texture* displacementMap = TextureManager::Get()->GetTexture(displacementTextureId);

			DepthMapOptionData optionData;
			if (displacementMap != nullptr)
			{
				//bind 
				displacementMap->BindVS(0);
				optionData.displacementWeight = mEffectedObjects[index]->mEffectContext->GetDisplacementMapWeight();
				optionData.isUsingDisplacementMap = 1;
			}
			

			if (mEffectedObjects[index]->mEffectContext->GetEffectContextType() == EffectType::SkeletonModelMesh) // Standard mesh may have implement displacement map
			{
				optionData.isUsingSkeleton = 1;
				Effects::SkeletonModelMeshEffect::SkeletonModelMeshEffectContext* skeletonEffectContext =
					dynamic_cast<Effects::SkeletonModelMeshEffect::SkeletonModelMeshEffectContext*>(mEffectedObjects[index]->mEffectContext);

				mBoneTransformBuffer.Load(skeletonEffectContext->mBoneTransformMatrix);
				mBoneTransformBuffer.BindVS(2);
			}

			mDepthMapOptionBuffer.Load(optionData);
			mDepthMapOptionBuffer.BindVS(1);

			mDepthContantBuffer.Load(wvp);
			mDepthContantBuffer.BindVS(0);
			// Bind displacement map here for more accurate shadow
			mEffectedObjects[index]->mMeshBuffer->Render();
		}
		index++;
	}
	
	mRenderTarget.EndRender();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::PostProcess()
{
	auto postProcessManager = PostProcessManager::Get();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::OptionUI()
{
	ImGui::Image(
		mRenderTarget.GetShaderResourceView(),
		{ 150.0f,150.0f },
		{ 0.0f,0.0f },
		{ 1.0f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f }
	);
	ImGui::DragFloat("Depth Bias", &mDepthBias, 0.0001f, 0.0f, NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferWidth(),"%.4f");
	ImGui::DragFloat("Shadow Cover Range", &mShadowCoverRange, 0.01f, 0.0f, 1.0f, "%.2f");
}

std::string NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::GetName()
{
	return "PostProcessUnit_Shadow";
}

uint32_t NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::GetContextTypeMask()
{
	return PostProcessUnit_Shadow::ContextTypeMask;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_Shadow::RegisterAffectObject(RenderContext * renderObject)
{
	if (renderObject->mEffectContext->GetPostProcessContextMask() & PostProcessContext::Type_SHADOW_02)
	{
		mEffectedObjects.push_back(renderObject);
		auto effectContext = renderObject->mEffectContext;
		PostProcessContext_Shadow* contextPtr = dynamic_cast<PostProcessContext_Shadow*>(effectContext->effectedPostProcessContext[effectContext->contextIndexTracker[PostProcessContext::Type_SHADOW_02]].get());
		ASSERT(contextPtr, "Miss match postProcessContext in passed in effectContext");
		mEffectedObjectContext.push_back(contextPtr); 
	}
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- PostProcessUnit_Shadow ----------------------------------------------------------------
// -------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------
// --- PostProcessUnit_DepthBlur -------------------------------------------------------------
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// ---vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv---
PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::sGetType()
{
	return PostProcessType::DepthBlur;
}

PostProcessType NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::GetType()
{
	return PostProcessType::DepthBlur;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::Initialize()
{
	PostProcessUnit::Initialize();
	mEffectedObjectContext.reserve(EffectManager::InitialRenderObjectCapacity);
	auto graphicSystem = GraphicsSystem::Get();
	mScreenMesh = MeshBuilder::CreateNDCQuad();
	mScreenMeshBuffer.Initialize(mScreenMesh);
	mRenderTarget.Initialize(graphicSystem->GetBackBufferWidth(), graphicSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U32);
	mDepthMapRenderTarget.Initialize(graphicSystem->GetBackBufferWidth(), graphicSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);

	mPostProcessVertexShader_pre.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessDepthBlur_0).wstring().c_str(), BoneVertex::Format);
	mPostProcessPixelShader_pre.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessDepthBlur_0).wstring().c_str());

	mPostProcessVertexShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessDepthBlur_1).wstring().c_str(), VertexPX::Format);
	mPostProcessPixelShader.Initialize((PostProcessManager::Get()->GetRootPath() / PostProcessDepthBlur_1).wstring().c_str());
	
	mBoneTransformBuffer.Initialize();
	mDepthConstantBuffer.Initialize();
	mDepthMapOptionBuffer.Initialize();
	mDepthBlurOptionBuffer.Initialize();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::Terminate()
{
	mDepthBlurOptionBuffer.Terminate();
	mDepthMapOptionBuffer.Terminate();
	mDepthConstantBuffer.Terminate();
	mBoneTransformBuffer.Terminate();

	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mPostProcessPixelShader_pre.Terminate();
	mPostProcessVertexShader_pre.Terminate();
	mRenderTarget.Terminate();
	mDepthMapRenderTarget.Terminate();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::Active()
{
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::Disable()
{
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::PreProcess()
{
	mDepthMapRenderTarget.BeginRender();
	mPostProcessVertexShader_pre.Bind();
	mPostProcessPixelShader_pre.Bind();
	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);

	int index = 0;
	for (auto depthBlurContext : mEffectedObjectContext)
	{
		auto effectContext = mEffectedObjects[index]->mEffectContext;
		TextureId displacementTextureId = 0;
		effectContext->GetTexture(MeshTextureMaterial::DISPLACEMENT, displacementTextureId);
		Texture* displacementMap = TextureManager::Get()->GetTexture(displacementTextureId);

		DepthMapOptionData optionData;
		if (displacementMap != nullptr)
		{
			//bind 
			displacementMap->BindVS(0);
			optionData.displacementWeight = effectContext->GetDisplacementMapWeight();
			optionData.isUsingDisplacementMap = 1;
		}


		if (effectContext->GetEffectContextType() == EffectType::SkeletonModelMesh) // Standard mesh may have implement displacement map
		{
			optionData.isUsingSkeleton = 1;
			Effects::SkeletonModelMeshEffect::SkeletonModelMeshEffectContext* skeletonEffectContext =
				dynamic_cast<Effects::SkeletonModelMeshEffect::SkeletonModelMeshEffectContext*>(effectContext);

			mBoneTransformBuffer.Load(skeletonEffectContext->mBoneTransformMatrix);
			mBoneTransformBuffer.BindVS(2);
		}

		DepthMapTransformData transformData;
		auto currentUseCamera = PostProcessManager::Get()->GetCurrentUseCamera();
		TransformData contextTransformData = effectContext->GetTransformData();
		transformData.world = contextTransformData.world;
		transformData.wvp = contextTransformData.wvp;
		transformData.view = NFGE::Math::Transpose(currentUseCamera->GetViewMatrix());
		transformData.cameraDepth = mEffectRange;

		mDepthMapOptionBuffer.Load(optionData);
		mDepthMapOptionBuffer.BindVS(1);

		mDepthConstantBuffer.Load(transformData);
		mDepthConstantBuffer.BindVS(0);
		mEffectedObjects[index]->mMeshBuffer->Render();

		index++;
	}

	mDepthMapRenderTarget.EndRender();
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::PostProcess()
{
	auto postProcessManager = PostProcessManager::Get();
	auto lastRenderTargetPtr = postProcessManager->GetLastRenderTargetPtr();
	mRenderTarget.BeginRender();
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();
	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);
	
	DepthBlurOptionData optionData;
	optionData.focusDepth = mFocusDepth;
	optionData.blurPower = mBlurPower;
	optionData.blurRange = mBlurRange;
	optionData.screenWidth = static_cast<float>(NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferWidth());
	optionData.screenHeight = static_cast<float>(NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferHeight());

	mDepthBlurOptionBuffer.Load(optionData);
	mDepthBlurOptionBuffer.BindPS(0);
	lastRenderTargetPtr->BindPS(0);
	mDepthMapRenderTarget.BindPS(1);
	mScreenMeshBuffer.Render();
	mDepthMapRenderTarget.UnbindPS(1);
	lastRenderTargetPtr->UnbindPS(0);
	mRenderTarget.EndRender();
	postProcessManager->SetLastRenderTargetPtr(&mRenderTarget);
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::OptionUI()
{
	ImGui::Image(
		mDepthMapRenderTarget.GetShaderResourceView(),
		{ 150.0f,150.0f },
		{ 0.0f,0.0f },
		{ 1.0f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f }
	);

	ImGui::DragFloat("Effect Range", &mEffectRange, 10.0f, 0.0f, 5000.0f);
	ImGui::DragFloat("Focus Depth", &mFocusDepth, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Blur Range", &mBlurRange, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Blur Power", &mBlurPower, 2.0f, 0.0f, 100.0f);
}

std::string NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::GetName()
{
	return "PostProcessUnit_DepthBlur";
}

uint32_t NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::GetContextTypeMask()
{
	return PostProcessUnit_DepthBlur::ContextTypeMask;
}

void NFGE::Graphics::PostProcesses::PostProcessUnit_DepthBlur::RegisterAffectObject(RenderContext * renderObject)
{
	if (renderObject->mEffectContext->GetPostProcessContextMask() & PostProcessContext::Type_DepthBlur_03)
	{
		mEffectedObjects.push_back(renderObject);
		auto effectContext = renderObject->mEffectContext;
		PostProcessContext_DepthBlur* contextPtr = dynamic_cast<PostProcessContext_DepthBlur*>(effectContext->effectedPostProcessContext[effectContext->contextIndexTracker[PostProcessContext::Type_DepthBlur_03]].get());
		ASSERT(contextPtr, "Miss match postProcessContext in passed in effectContext");
		mEffectedObjectContext.push_back(contextPtr);
	}
}

// ---^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^---
// ---|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||---
// --- PostProcessUnit_DepthBlur -------------------------------------------------------------
// -------------------------------------------------------------------------------------------