#ifndef NFGEEDITOR_POSTPROCESSSER_HEADER
#define NFGEEDITOR_POSTPROCESSSER_HEADER

#include <NFGE/Inc/NFGE.h>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

class PostPorcesser
{
public:
	virtual void Initialize() = 0;
	virtual void Terminate()
	{
		for (size_t i= 0; i < mRenderTargets.size(); i++)
		{
			mRenderTargets[i]->Terminate();
			delete mRenderTargets[i];
		}
	}
	virtual void Prepare(int step) = 0;
	virtual void Render() = 0;
	virtual void DebugUI() = 0;

public:
	int step;

	std::vector<NFGE::Graphics::RenderTarget*> mRenderTargets;

	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;
};

class PostProcesser_DepthBlur : public PostPorcesser
{
	struct OptionsData
	{
		float focusDepth;
		float padding[3];
		float blurRange;
		float padding1[3];
		float blurPower;
		float padding2[3];
		float screenWidth;
		float screenHeight;
		float padding3[2];
	};
	using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;

public:
	virtual void Initialize() override;
	virtual void Terminate() override;
	virtual void Prepare(int step) override;
	virtual void Render() override;
	virtual void DebugUI() override;

	float focusDepth = 0.2f;
	float blurRange = 0.07f;
	float blurPower = 10.0f;

	VertexShader mPostProcessVertexShader;
	PixelShader mPostProcessPixelShader;
	VertexShader mPostProcessVertexShader_GetDepth;
	PixelShader mPostProcessPixelShader_GetDepth;
	OptionBuffer mOptionDateBuffer;

};

#endif // !HELLOPOSTPROCESSADVANCE_POSTPROCESSSER_HEADER
