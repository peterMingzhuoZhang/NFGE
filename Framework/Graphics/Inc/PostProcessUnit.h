#ifndef INCLUDE_NFGE_GRAPHICS_POSTPROCESSEDUNIT_H
#define INCLUDE_NFGE_GRAPHICS_POSTPROCESSEDUNIT_H

#include "Mesh.h"
#include "Camera.h"
#include "MeshBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"

namespace NFGE::Graphics
{ 
	enum class PostProcessType
	{
		None,
		Pixelate,
		Shadow,
		DepthBlur
	};
	struct RenderContext;
	struct PostProcessContext 
	{
		static const uint32_t TypeCount = 4;
		static const uint32_t Type_NONE_00			= 0x1 << 0;
		static const uint32_t Type_PIXELATE_01		= 0x1 << 1;
		static const uint32_t Type_SHADOW_02		= 0x1 << 2;
		static const uint32_t Type_DepthBlur_03		= 0x1 << 3;

		virtual void DebugUI() = 0;
	};
	class PostProcessUnit 
	{
	public:
		virtual PostProcessType GetType() = 0;

		virtual void Initialize();
		virtual void Terminate() = 0;
		virtual void Active() = 0;
		virtual void Disable() = 0;
		virtual void PreProcess() = 0;
		virtual void PostProcess() = 0;
		virtual void OptionUI() = 0;
		virtual std::string GetName() = 0;
		virtual uint32_t GetContextTypeMask() = 0;
		virtual void RegisterAffectObject(RenderContext* renderObject) = 0;//{ mEffectedObject.push_back(renderObject); }
		virtual void ClearRenderObject() = 0;
	protected:
		std::vector<RenderContext*> mEffectedObjects;

		MeshPX mScreenMesh;
		MeshBuffer mScreenMeshBuffer;
		VertexShader mPostProcessVertexShader;
		PixelShader mPostProcessPixelShader;
		NFGE::Graphics::RenderTarget mRenderTarget;
	};

	namespace PostProcesses
	{
		class PostProcessUnit_None : public PostProcessUnit
		{
		public:
			struct PostProcessContext_None : NFGE::Graphics::PostProcessContext
			{
				void DebugUI() override{};
			};
		public:
			static const uint32_t ContextTypeMask = PostProcessContext::Type_NONE_00;
			static PostProcessType sGetType();
		public:
			PostProcessType GetType() override;
			void Initialize() override ;
			void Terminate() override;
			void Active() override {};
			void Disable() override {};
			//virtual void AddEffectedObject(PostProcessContext* objectContext) = 0;
			void PreProcess() override;
			void PostProcess() override;
			void OptionUI() override;
			std::string GetName() override;
			uint32_t GetContextTypeMask() override;
			void RegisterAffectObject(RenderContext* renderObject) override { }
			void ToScreenRender();
			void ClearRenderObject() override { mEffectedObjects.clear(); mEffectedObjectContext.clear(); };
		private:
			std::vector<PostProcessContext_None*> mEffectedObjectContext;
		};

		class PostProcessUnit_Pixelate : public PostProcessUnit
		{
		public:
			struct PixelateOptionData
			{
				float pixelatePower = 1.0f;
				float padding[3];
			};
			using PixelateOptionBuffer = TypedConstantBuffer<PixelateOptionData>;
			struct PostProcessContext_Pixelate : NFGE::Graphics::PostProcessContext
			{
				void DebugUI() override {};
			};
		public:
			static const uint32_t ContextTypeMask = PostProcessContext::Type_PIXELATE_01;
			static PostProcessType sGetType();
		public:
			PostProcessType GetType() override;
			void Initialize() override;
			void Terminate() override;
			void Active() override {};
			void Disable() override {};
			//virtual void AddEffectedObject(PostProcessContext* objectContext) = 0;
			void PreProcess() override;
			void PostProcess() override;
			void OptionUI() override;
			std::string GetName() override;
			uint32_t GetContextTypeMask() override;
			void RegisterAffectObject(RenderContext* renderObject) override;
			void ClearRenderObject() override { mEffectedObjects.clear(); mEffectedObjectContext.clear(); };
		private:
			std::vector<PostProcessContext_Pixelate*> mEffectedObjectContext;

			PixelateOptionBuffer mPixelateOptionBuffer;
			float mPixelatePower = 500.0f;
		};

		class PostProcessUnit_Shadow : public PostProcessUnit
		{
		public:
			struct ShadowOptionData
			{
				float depthBias;
				float padding[3];
			}; 

			struct DepthMapOptionData
			{
				int isUsingDisplacementMap = 0;
				float displacementWeight = 0.0f;
				int isUsingSkeleton = 0;
				float padding;
			};
			using ShadowOptionBuffer = TypedConstantBuffer<ShadowOptionData>;
			using DepthMapOptionBuffer = TypedConstantBuffer<DepthMapOptionData>;
			using ShadowConstantBuffer = TypedConstantBuffer<NFGE::Math::Matrix4>;
			using BoneTransformBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Math::Matrix4[128]>;
			using DepthConstantBuffer = TypedConstantBuffer<NFGE::Math::Matrix4>;

			struct PostProcessContext_Shadow : NFGE::Graphics::PostProcessContext
			{
				Math::Matrix4 toWorldMatrix;
				Math::Matrix4 readyToUseLightWvp;
				bool isCastShadow = true;
				void DebugUI() override 
				{ 
					ImGui::BeginGroup();
					ImGui::Checkbox("isCastShadow", &isCastShadow); 
					ImGui::EndGroup();
				};
			};
		public:
			static const uint32_t ContextTypeMask = PostProcessContext::Type_SHADOW_02;
			static PostProcessType sGetType();
		public:
			PostProcessType GetType() override;
			void Initialize() override;
			void Terminate() override;
			void Active() override;
			void Disable() override;
			//virtual void AddEffectedObject(PostProcessContext* objectContext) = 0;
			void PreProcess() override;
			void PostProcess() override;
			void OptionUI() override;
			std::string GetName() override;
			uint32_t GetContextTypeMask() override;
			void RegisterAffectObject(RenderContext* renderObject) override;
			void ClearRenderObject() override { mEffectedObjects.clear(); mEffectedObjectContext.clear(); };

			float GetDepthBias() const { return mDepthBias; }
			const Math::Matrix4& GetLightViewMat() const { return mLightViewMat; }
			const Math::Matrix4& GetLightProjMat() const {return mLightProjMat; }
			NFGE::Graphics::RenderTarget* GetRenderTarget() { return &mRenderTarget; }
		private:
			std::vector<PostProcessContext_Shadow*> mEffectedObjectContext;

			Camera mLightCamera;
			Math::Matrix4 mLightViewMat;
			Math::Matrix4 mLightProjMat;
			DepthConstantBuffer mDepthContantBuffer;
			BoneTransformBuffer mBoneTransformBuffer;
			DepthMapOptionBuffer mDepthMapOptionBuffer;
			float mDepthBias = 0.0003f;
			float mShadowCoverRange = 1.0f; // 0.0f-1.0f
		};

		class PostProcessUnit_DepthBlur : public PostProcessUnit
		{
		public:
			struct DepthBlurOptionData
			{
				float focusDepth;
				float blurRange;
				float blurPower;
				float screenWidth;
				float screenHeight;
				float padding[3];
			};

			struct DepthMapOptionData
			{
				int isUsingDisplacementMap = 0;
				float displacementWeight = 0.0f;
				int isUsingSkeleton = 0;
				float padding;
			};

			struct DepthMapTransformData
			{
				NFGE::Math::Matrix4 world;
				NFGE::Math::Matrix4 view;
				NFGE::Math::Matrix4 wvp;
				float cameraDepth;
				float padding[3];
			};

			using BoneTransformBuffer = TypedConstantBuffer<NFGE::Math::Matrix4[128]>;
			using DepthConstantBuffer = TypedConstantBuffer<DepthMapTransformData>;
			using DepthMapOptionBuffer = TypedConstantBuffer<DepthMapOptionData>;
			using DepthBlurOptionBuffer = TypedConstantBuffer<DepthBlurOptionData>;

			struct PostProcessContext_DepthBlur : NFGE::Graphics::PostProcessContext
			{
				void DebugUI() override
				{
				};
			};
		public:
			static const uint32_t ContextTypeMask = PostProcessContext::Type_DepthBlur_03;
			static PostProcessType sGetType();
		public:
			PostProcessType GetType() override;
			void Initialize() override;
			void Terminate() override;
			void Active() override;
			void Disable() override;
			//virtual void AddEffectedObject(PostProcessContext* objectContext) = 0;
			void PreProcess() override;
			void PostProcess() override;
			void OptionUI() override;
			std::string GetName() override;
			uint32_t GetContextTypeMask() override;
			void RegisterAffectObject(RenderContext* renderObject) override;
			void ClearRenderObject() override { mEffectedObjects.clear(); mEffectedObjectContext.clear(); };

			NFGE::Graphics::RenderTarget* GetRenderTarget() { return &mRenderTarget; }

			float GetEffectRange() const { return mEffectRange; }
			void SetEffectRange(float newRange) { mEffectRange = newRange; }
			float GetFocusDepth() const { return mFocusDepth; }
			void SetFocusDepth(float newValue) { mFocusDepth = newValue; }
			float GetBlurRange() const { return mBlurRange; }
			void SetBlurRange(float newRange) { mBlurRange = newRange; }
			float GetBlurPower() const { return mBlurPower; }
			void SetBlurPower(float newValue) { mBlurPower  = newValue; }
			
		private:
			std::vector<PostProcessContext_DepthBlur*> mEffectedObjectContext;
			
			VertexShader mPostProcessVertexShader_pre;
			PixelShader mPostProcessPixelShader_pre;
			
			BoneTransformBuffer mBoneTransformBuffer;
			DepthConstantBuffer mDepthConstantBuffer;
			DepthMapOptionBuffer mDepthMapOptionBuffer;
			DepthBlurOptionBuffer mDepthBlurOptionBuffer;

			NFGE::Graphics::RenderTarget mDepthMapRenderTarget;

			float mEffectRange = 2000.0f;
			float mFocusDepth = 0.2f; // percentage
			float mBlurRange = 0.07f; // percentage
			float mBlurPower = 10.0f; // 0 - 100.0f
		};
	}

}

#endif // !INCLUDE_NFGE_GRAPHICS_POSTPROCESSEDUNIT_H

