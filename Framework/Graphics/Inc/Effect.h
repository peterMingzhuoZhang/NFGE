#ifndef INCLUDE_NFGE_GRAPHICS_EFFECT_H
#define INCLUDE_NFGE_GRAPHICS_EFFECT_H

#include "GeometryShader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "MeshTextureMaterial.h"
#include "SamplerManager.h"
#include "Light.h"
#include "Material.h"
#include "Skeleton.h"
#include "PostProcessManager.h"

namespace NFGE::Graphics
{
	class Camera;
	
	class MeshBuffer;
	struct EffectContext;

	struct TransformData
	{
		NFGE::Math::Matrix4 world;
		NFGE::Math::Matrix4 wvp;
		NFGE::Math::Vector3 viewPositon;
		float padding;
	};
	struct OptionsData
	{
		float displacementWeight;
		uint32_t meshTextureInfoMask;
		int useShadow;
		float depthBias;
	};
	using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::DirectionalLight>;
	using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
	using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;

	enum class EffectType
	{
		OnScreen,
		ColorMesh,
		TextureMesh,
		StandardMesh,
		ModelMesh,
		SkeletonModelMesh,
	};

	struct EffectContext
	{
		std::string rasterizerState = "Solid";

		std::vector<std::unique_ptr<PostProcessContext>> effectedPostProcessContext;
		std::map< uint32_t, uint32_t > contextIndexTracker;

		virtual uint32_t GetPostProcessContextMask() = 0;
		virtual EffectType GetEffectContextType() = 0;
		virtual void GetTexture(MeshTextureMaterial::ModelTextureType textureType, TextureId& textureId) { textureId = 0; }
		virtual float GetDisplacementMapWeight() { return 0.0f; }
		virtual const TransformData& GetTransformData() { ASSERT(false, "Current Effect not support this func"); return TransformData(); }
	};

	class Effect
	{
	public:

		virtual void Set(EffectContext* contextPtr) = 0;
		virtual void Bind(const Camera& camera) = 0;
	private:
		friend class EffectManager;
		virtual void Initialize() = 0;
		virtual void Terminate() = 0;
		virtual void AfterUseReset() = 0;
	protected:
		friend class EffectManager;
		

	};
	namespace Effects
	{
		class OnScreenEffect : public Effect
		{
		public:
			struct OnScreenEffectContext : public EffectContext
			{
				static const uint32_t PostProcessContextMask = PostProcessContext::Type_NONE_00;
				uint32_t GetPostProcessContextMask() override { return OnScreenEffectContext::PostProcessContextMask; }
				EffectType GetEffectContextType() override { return EffectType::OnScreen; }
				OnScreenEffectContext()
				{
					effectedPostProcessContext.reserve(PostProcessContext::TypeCount);
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_None::PostProcessContext_None>());
					contextIndexTracker. emplace(PostProcessContext::Type_NONE_00, 0);
				}
			}* mContext;
		public:
			void Set(EffectContext* contextPtr) override;
			void Bind(const Camera& camera) override;
		private:
			friend class EffectManager;
			void Initialize() override;
			void Terminate() override;
			virtual void AfterUseReset() override;

		private:

			PixelShader mPixelShader;
			VertexShader mVertexShader;
		};

		class ColorMeshEffect : public Effect
		{
		public:
			struct ColorMeshEffectContext : public EffectContext
			{
				static const uint32_t PostProcessContextMask =	PostProcessContext::Type_PIXELATE_01 |
																PostProcessContext::Type_DepthBlur_03;
				uint32_t GetPostProcessContextMask() override { return ColorMeshEffectContext::PostProcessContextMask; }
				EffectType GetEffectContextType() override { return EffectType::ColorMesh; }
				float GetDisplacementMapWeight() override { return 0.0f; }
				ColorMeshEffectContext()
				{
					effectedPostProcessContext.reserve(PostProcessContext::TypeCount);
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Pixelate::PostProcessContext_Pixelate>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_DepthBlur::PostProcessContext_DepthBlur>());
					contextIndexTracker.emplace(PostProcessContext::Type_PIXELATE_01, 0);
					contextIndexTracker.emplace(PostProcessContext::Type_DepthBlur_03, 1);
				}

				NFGE::Math::Vector3 position = NFGE::Math::Vector3::Zero();
				NFGE::Math::Quaternion rotation = NFGE::Math::Quaternion::Identity();
				NFGE::Math::Vector3 scale = NFGE::Math::Vector3::One();

				TransformData mTransformData;
				const TransformData& GetTransformData() override { return mTransformData; }
			}* mContext;
		public:
			void Set(EffectContext* contextPtr) override;
			void Bind(const Camera& camera) override;
		private:
			friend class EffectManager;
			void Initialize() override;
			void Terminate() override;
			virtual void AfterUseReset() override;

		private:
			PixelShader mPixelShader;
			VertexShader mVertexShader;
			ConstantBuffer mConstantBuffer;
		};

		class TextureMeshEffect : public Effect
		{
		public:
			struct TextureMeshEffectContext : public EffectContext
			{
				static const uint32_t PostProcessContextMask =	PostProcessContext::Type_PIXELATE_01 |
																PostProcessContext::Type_DepthBlur_03;
				uint32_t GetPostProcessContextMask() override { return TextureMeshEffectContext::PostProcessContextMask; }
				EffectType GetEffectContextType() override { return EffectType::TextureMesh; }
				float GetDisplacementMapWeight() override { return 0.0f; }
				TextureMeshEffectContext()
				{
					effectedPostProcessContext.reserve(PostProcessContext::TypeCount);
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Pixelate::PostProcessContext_Pixelate>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_DepthBlur::PostProcessContext_DepthBlur>());
					contextIndexTracker.emplace(PostProcessContext::Type_PIXELATE_01, 0);
					contextIndexTracker.emplace(PostProcessContext::Type_DepthBlur_03, 1);
				}

				NFGE::Math::Vector3 position = NFGE::Math::Vector3::Zero();
				NFGE::Math::Quaternion rotation = NFGE::Math::Quaternion::Identity();
				NFGE::Math::Vector3 scale = NFGE::Math::Vector3::One();

				TextureId textureId;

				TransformData mTransformData;
				const TransformData& GetTransformData() override { return mTransformData; }

			}*mContext;
		public:
			void Set(EffectContext* contextPtr) override;
			void Bind(const Camera& camera) override;
		private:
			friend class EffectManager;
			void Initialize() override;
			void Terminate() override;
			virtual void AfterUseReset() override;

		private:
			VertexShader mVertexShader;
			PixelShader mPixelShader;
			ConstantBuffer mConstantBuffer;
			Sampler* sampler;
		};

		class StandardMeshEffect : public Effect
		{
		public:
			struct StandardMeshEffectContext : public EffectContext
			{
				static const uint32_t PostProcessContextMask =	PostProcessContext::Type_NONE_00 | 
																PostProcessContext::Type_PIXELATE_01 | 
																PostProcessContext::Type_SHADOW_02 |
																PostProcessContext::Type_DepthBlur_03;
				uint32_t GetPostProcessContextMask() override { return StandardMeshEffectContext::PostProcessContextMask; }
				EffectType GetEffectContextType() override { return EffectType::StandardMesh; }
				StandardMeshEffectContext()
				{
					//effectedPostProcessContext.reserve(PostProcessContext::TypeCount);
					effectedPostProcessContext.reserve(PostProcessContext::TypeCount); // Hard code number for memory compacity Optimazation
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_None::PostProcessContext_None>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Pixelate::PostProcessContext_Pixelate>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_DepthBlur::PostProcessContext_DepthBlur>());
					contextIndexTracker.emplace(PostProcessContext::Type_NONE_00, 0);
					contextIndexTracker.emplace(PostProcessContext::Type_PIXELATE_01, 1);
					contextIndexTracker.emplace(PostProcessContext::Type_SHADOW_02, 2);
					contextIndexTracker.emplace(PostProcessContext::Type_DepthBlur_03, 3);

					textureTypes.reserve(MeshTextureMaterial::totalTypeCount);
					textures.reserve(MeshTextureMaterial::totalTypeCount);

					for (size_t i = 0; i < MeshTextureMaterial::totalTypeCount; ++i)
					{
						usingTextureSwitch |= 0x1 << i;
					}
				}
				
				NFGE::Math::Matrix4 custumAdjustMatrix = NFGE::Math::Matrix4::sIdentity();
				NFGE::Math::Vector3 AdjustPosition = NFGE::Math::Vector3::Zero();
				NFGE::Math::Vector3 AdjustRotation = NFGE::Math::Vector3::Zero();
				NFGE::Math::Vector3 AdjustScale = NFGE::Math::Vector3::One();

				NFGE::Math::Matrix4 custumToWorldMatrix = NFGE::Math::Matrix4::sIdentity();
				NFGE::Math::Vector3 position = NFGE::Math::Vector3::Zero();
				NFGE::Math::Quaternion rotation = NFGE::Math::Quaternion::Identity();
				NFGE::Math::Vector3 scale = NFGE::Math::Vector3::One();

				std::vector<MeshTextureMaterial::ModelTextureType> textureTypes;
				std::vector<TextureId> textures;
				std::array<int, MeshTextureMaterial::totalTypeCount> textureIndexFinder;
				uint32_t currentContainTexture = 0;
				uint32_t usingTextureSwitch;

				DirectionalLight* light;
				Material material{Colors::Gray,Colors::Gray, Colors::Gray, 1.0f};
				float bumpWeight = 0.0f;
				
				TransformData mTransformData;

				void AddTexture(MeshTextureMaterial::ModelTextureType textureType, TextureId textureId)
				{
					auto temp = currentContainTexture & (0x1 << (uint32_t)textureType);

					if (currentContainTexture & (0x1 << (uint32_t)textureType))	// Add same type texture means replace it
					{
						textures[textureIndexFinder[(uint32_t)textureType]] = textureId;
					}
					else
					{
						currentContainTexture |= 0x1 << (uint32_t)textureType;
						textureIndexFinder[(uint32_t)textureType] = textures.size();
						textureTypes.push_back(textureType);
						textures.push_back(textureId);
					}
				}

				void GetTexture(MeshTextureMaterial::ModelTextureType textureType, TextureId& textureId) override
				{
					if (textures.empty())
					{
						textureId = 0;
						return;
					}
					textureId = textures[textureIndexFinder[textureType]];
				}

				void TextureUsingSwitch(MeshTextureMaterial::ModelTextureType textureType, bool flip) 
				{
					if (flip)
					{
						usingTextureSwitch |= 0x1 << (uint32_t)textureType;
					}
					else
					{
						usingTextureSwitch &= ~(0x1 << (uint32_t)textureType);
					}
				}

				float GetDisplacementMapWeight() override { return bumpWeight; }
				const TransformData& GetTransformData() override { return mTransformData; }
				PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow* GetShadowPostProcessContextPtr() { return (PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow*)effectedPostProcessContext[contextIndexTracker[PostProcessContext::Type_SHADOW_02]].get(); }
			}*mContext;
		public:
			void Set(EffectContext* contextPtr) override;
			void Bind(const Camera& camera) override;
		private:
			friend class EffectManager;
			void Initialize() override;
			void Terminate() override;
			virtual void AfterUseReset() override;

		private:
			TransformBuffer mTransformBuffer;
			LightBuffer mLightBuffer;
			MaterialBuffer mMaterialBuffer;
			OptionBuffer mOptionDateBuffer;
			PostProcesses::PostProcessUnit_Shadow::ShadowConstantBuffer mShadowConstantBuffer;

			VertexShader mVertexShader;
			PixelShader mPixelShader;
			Sampler* sampler;
		};

		class ModelMeshEffect : public Effect
		{
		public:
			struct ModelMeshEffectContext : public EffectContext
			{
				static const uint32_t PostProcessContextMask =	PostProcessContext::Type_NONE_00 |
																PostProcessContext::Type_PIXELATE_01 |
																PostProcessContext::Type_SHADOW_02 |
																PostProcessContext::Type_DepthBlur_03;
				uint32_t GetPostProcessContextMask() override { return ModelMeshEffectContext::PostProcessContextMask; }
				EffectType GetEffectContextType() override { return EffectType::ModelMesh; }
				ModelMeshEffectContext()
				{
					//effectedPostProcessContext.reserve(PostProcessContext::TypeCount);
					effectedPostProcessContext.reserve(PostProcessContext::TypeCount);
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_None::PostProcessContext_None>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Pixelate::PostProcessContext_Pixelate>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_DepthBlur::PostProcessContext_DepthBlur>());
					contextIndexTracker.emplace(PostProcessContext::Type_NONE_00, 0);
					contextIndexTracker.emplace(PostProcessContext::Type_PIXELATE_01, 1);
					contextIndexTracker.emplace(PostProcessContext::Type_SHADOW_02, 2);
					contextIndexTracker.emplace(PostProcessContext::Type_DepthBlur_03, 3);
				}

				NFGE::Math::Matrix4 custumAdjustMatrix = NFGE::Math::Matrix4::sIdentity();
				NFGE::Math::Vector3 AdjustPosition = NFGE::Math::Vector3::Zero();
				NFGE::Math::Vector3 AdjustRotation = NFGE::Math::Vector3::Zero();
				NFGE::Math::Vector3 AdjustScale = NFGE::Math::Vector3::One();

				NFGE::Math::Matrix4 custumToWorldMatrix = NFGE::Math::Matrix4::sIdentity();
				NFGE::Math::Vector3 position = NFGE::Math::Vector3::Zero();
				NFGE::Math::Quaternion rotation = NFGE::Math::Quaternion::Identity();
				NFGE::Math::Vector3 scale = NFGE::Math::Vector3::One();

				NFGE::Math::Vector3 currentFoward = NFGE::Math::Vector3{ 0.0f,0.0f,1.0f };

				std::vector<MeshTextureMaterial> modelMaterials;
				std::vector<size_t> materialIndices;

				DirectionalLight* light;
				std::vector<Material> materials;
				float bumpWeight = 0.0f;

				TransformData mTransformData;

				void GetTexture(MeshTextureMaterial::ModelTextureType textureType, TextureId& textureId) override
				{
					textureId = modelMaterials[0].GetTexture(textureType);
				}
				float GetDisplacementMapWeight() override { return bumpWeight; }
				const TransformData& GetTransformData() override { return mTransformData; }
				PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow* GetShadowPostProcessContextPtr() { return (PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow*)effectedPostProcessContext[contextIndexTracker[PostProcessContext::Type_SHADOW_02]].get(); }

			}*mContext;
		public:
			void Set(EffectContext* contextPtr) override;
			void Bind(const Camera& camera) override;
		private:
			friend class EffectManager;
			void Initialize() override;
			void Terminate() override;
			virtual void AfterUseReset() override;

		private:
			TransformBuffer mTransformBuffer;
			LightBuffer mLightBuffer;
			MaterialBuffer mMaterialBuffer;
			OptionBuffer mOptionDateBuffer;
			PostProcesses::PostProcessUnit_Shadow::ShadowConstantBuffer mShadowConstantBuffer;

			VertexShader mVertexShader;
			PixelShader mPixelShader;
			Sampler* sampler;

			EffectContext* mLastContextPtr = nullptr;
			size_t mMeshIndex = 0;
			bool mIsNeedFullBind = false;	// Model may have muti mesh, render different different mesh dont need re bind everything.
											// Toggle this variable allows skiping unnessarary bind
		};

		class SkeletonModelMeshEffect : public Effect
		{
		public:
			using BoneTransformBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Math::Matrix4[128]>;
		public:
			struct SkeletonModelMeshEffectContext : public EffectContext
			{
				static const uint32_t PostProcessContextMask =	PostProcessContext::Type_NONE_00 |
																PostProcessContext::Type_PIXELATE_01 |
																PostProcessContext::Type_SHADOW_02 |
																PostProcessContext::Type_DepthBlur_03;
				uint32_t GetPostProcessContextMask() override { return SkeletonModelMeshEffectContext ::PostProcessContextMask; }
				EffectType GetEffectContextType() override { return EffectType::SkeletonModelMesh; }
				
				SkeletonModelMeshEffectContext(Skeleton& skeleton)
					:mSkeleton(skeleton)
				{
					//effectedPostProcessContext.reserve(PostProcessContext::TypeCount);
					effectedPostProcessContext.reserve(3); // Hard code number for memory compacity Optimazation
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_None::PostProcessContext_None>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Pixelate::PostProcessContext_Pixelate>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow>());
					effectedPostProcessContext.emplace_back(std::make_unique<PostProcesses::PostProcessUnit_DepthBlur::PostProcessContext_DepthBlur>());
					contextIndexTracker.emplace(PostProcessContext::Type_NONE_00, 0);
					contextIndexTracker.emplace(PostProcessContext::Type_PIXELATE_01, 1);
					contextIndexTracker.emplace(PostProcessContext::Type_SHADOW_02, 2);
					contextIndexTracker.emplace(PostProcessContext::Type_DepthBlur_03, 3);
				}

				NFGE::Math::Matrix4 custumAdjustMatrix = NFGE::Math::Matrix4::sIdentity();
				NFGE::Math::Vector3 AdjustPosition = NFGE::Math::Vector3::Zero();
				NFGE::Math::Vector3 AdjustRotation = NFGE::Math::Vector3::Zero();
				NFGE::Math::Vector3 AdjustScale = NFGE::Math::Vector3::One();
				NFGE::Math::Matrix4 finalAdjustMatrix = NFGE::Math::Matrix4::sIdentity();

				NFGE::Math::Matrix4 custumToWorldMatrix = NFGE::Math::Matrix4::sIdentity();
				NFGE::Math::Vector3 position = NFGE::Math::Vector3::Zero();
				NFGE::Math::Quaternion rotation = NFGE::Math::Quaternion::Identity();
				NFGE::Math::Vector3 scale = NFGE::Math::Vector3::One();
				NFGE::Math::Matrix4 finalToWorld = NFGE::Math::Matrix4::sIdentity();

				NFGE::Math::Vector3 currentFoward = NFGE::Math::Vector3{ 0.0f,0.0f,1.0f };

				std::vector<MeshTextureMaterial> modelMaterials;
				std::vector<size_t> materialIndices;

				DirectionalLight* light;
				std::vector<Material> materials;
				float bumpWeight = 0.0f;

				// Bone
				Skeleton& mSkeleton;

				std::vector<NFGE::Math::Matrix4> tPosToParentMatrix;
				std::vector<NFGE::Math::Matrix4> boneMatrix;
				NFGE::Math::Matrix4 mBoneTransformMatrix[128];
				TransformData mTransformData;

				void UpdateTransform(int index);

				void SetTexture(MeshTextureMaterial::ModelTextureType textureType, TextureId textureId)
				{
					modelMaterials[0].LoadTexture( textureType, textureId);
				}

				void GetTexture(MeshTextureMaterial::ModelTextureType textureType, TextureId& textureId) override
				{
					textureId = modelMaterials[0].GetTexture(textureType);
				}
				float GetDisplacementMapWeight() override { return bumpWeight; }
				const TransformData& GetTransformData() override { return mTransformData; }
				PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow* GetShadowPostProcessContextPtr() { return (PostProcesses::PostProcessUnit_Shadow::PostProcessContext_Shadow*)effectedPostProcessContext[contextIndexTracker[PostProcessContext::Type_SHADOW_02]].get(); }
			private:
				void UpdateTransformImpl(Bone* bone, std::vector<NFGE::Math::Matrix4>& boneMatri);

			}*mContext;
		public:
			void Set(EffectContext* contextPtr) override;
			void Bind(const Camera& camera) override;
		private:
			friend class EffectManager;
			void Initialize() override;
			void Terminate() override;
			virtual void AfterUseReset() override;

		private:
			TransformBuffer mTransformBuffer;
			BoneTransformBuffer mBoneTransformBuffer;
			LightBuffer mLightBuffer;
			MaterialBuffer mMaterialBuffer;
			OptionBuffer mOptionDateBuffer;
			PostProcesses::PostProcessUnit_Shadow::ShadowConstantBuffer mShadowConstantBuffer;

			VertexShader mVertexShader;
			PixelShader mPixelShader;
			Sampler* sampler;

			EffectContext* mLastContextPtr = nullptr;
			size_t mMeshIndex = 0;
			bool mIsNeedFullBind = false;	// Model may have muti mesh, render different different mesh dont need re bind everything.
											// Toggle this variable allows skiping unnessarary bind

			//BoneMesh--From Hello Skeleton
		};
	}
}


#endif // !NFGE_GRAPHICS_EFFECT_H
