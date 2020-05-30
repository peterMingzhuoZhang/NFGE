#include "PostProcesser.h"

const wchar_t* postProcessShader_GetDepth = L"../../Assets/Shaders/PostProcess_GetDepth.fx";
const wchar_t* postProcessShader_DepthBlur = L"../../Assets/Shaders/PostProcess_DepthBlur.fx";


void PostProcesser_DepthBlur::Initialize()
{
	auto graphicSystem = GraphicsSystem::Get();
	mScreenMesh = MeshBuilder::CreateNDCQuad();
	mScreenMeshBuffer.Initialize(mScreenMesh);

	mPostProcessVertexShader.Initialize(postProcessShader_DepthBlur, NFGE::Graphics::VertexPX::Format);
	mPostProcessVertexShader_GetDepth.Initialize(postProcessShader_GetDepth, NFGE::Graphics::Vertex::Format);
	mPostProcessPixelShader.Initialize(postProcessShader_DepthBlur);
	mPostProcessPixelShader_GetDepth.Initialize(postProcessShader_GetDepth);

	mOptionDateBuffer.Initialize();
	step = 2;
	for (int i = 0; i < step; i++)
	{
		mRenderTargets.push_back(new NFGE::Graphics::RenderTarget());
		mRenderTargets.back()->Initialize(graphicSystem->GetBackBufferWidth(), graphicSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	}
	
}

void PostProcesser_DepthBlur::Terminate()
{
	PostPorcesser::Terminate();
	mScreenMeshBuffer.Terminate();

	mPostProcessVertexShader.Terminate();
	mPostProcessVertexShader_GetDepth.Terminate();
	mPostProcessPixelShader.Terminate();
	mPostProcessPixelShader_GetDepth.Terminate();
	mOptionDateBuffer.Terminate();
}

void PostProcesser_DepthBlur::Prepare(int step)
{
	mPostProcessVertexShader_GetDepth.Bind();
	mPostProcessPixelShader_GetDepth.Bind();
}

void PostProcesser_DepthBlur::Render()
{
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();

	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);

	for (int i = 0; i < step; i++)
	{
		mRenderTargets[i]->BindPS(i);
	}

	OptionsData optData;
	optData.focusDepth = focusDepth;
	optData.blurPower = blurPower;
	optData.blurRange = blurRange;
	optData.screenWidth = static_cast<float>(NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferWidth());
	optData.screenHeight= static_cast<float>(NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferHeight());

	mOptionDateBuffer.Load(optData);
	mOptionDateBuffer.BindPS(0);

	mScreenMeshBuffer.Render();
	for (int i = 0; i < step; i++)
	{
		mRenderTargets[i]->UnbindPS(i);
	}
}

void PostProcesser_DepthBlur::DebugUI()
{

	ImGui::Begin("PostProcesser_DepthBlur Setting");
	ImGui::DragFloat("Focus Depth", &focusDepth,0.01f,0.0f,1.0f);
	ImGui::DragFloat("Blur Range", &blurRange, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Blur Power", &blurPower, 2.0f, 0.0f, 100.0f);

	ImGui::End();
}
