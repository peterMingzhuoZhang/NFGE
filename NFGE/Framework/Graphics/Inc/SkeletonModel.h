#ifndef INCLUDE_NFGE_GRAGHICS_SKELETONMODEL_H
#define INCLUDE_NFGE_GRAGHICS_SKELETONMODEL_H

#include "Model.h"
#include "Skeleton.h"
#include "Animator.h"

namespace NFGE::Graphics
{
	struct DirectionalLight;
	class SkeletonModel
	{
	public:
		class BoneMesh
		{
		public:
			void Load(const NFGE::Math::Vector4& ambientColor, const NFGE::Math::Vector4& diffuseColor, DirectionalLight* light);
			void Render(const NFGE::Math::Matrix4& toParentMat, NFGE::Math::Matrix4& toWorldMat, const NFGE::Graphics::Camera& camera, bool isSelected);

			// Setter & Getter
			void SetDiffuseColor(const NFGE::Math::Vector4& color) { mDiffuseColor = color; }
			NFGE::Math::Vector4 GetDiffuseColor() const { return mDiffuseColor; }

			void SetAmbientColor(const NFGE::Math::Vector4& color) { mAmbientColor = color; }
			NFGE::Math::Vector4 GetAmbientColor() const { return mAmbientColor; }

		private:
			NFGE::Math::Vector4		mAmbientColor;
			NFGE::Math::Vector4		mDiffuseColor;
			DirectionalLight* mLight;
		};

	public:
		std::vector<Model::MeshData> mMeshData;
		Skeleton mSkeleton;
		int mDebugCurrentControlBone = 0;
		Effects::SkeletonModelMeshEffect::SkeletonModelMeshEffectContext mContext{mSkeleton};
		BoneMesh mBoneMesh;

	public:
		void Load(const char* modelFileName, const char* skeletonFileName, DirectionalLight* directionLight = nullptr);
		void Load(const std::filesystem::path& modelFileName, const std::filesystem::path& skeletonFileName, DirectionalLight* directionLight = nullptr);
		void UnLoad();
		void Update(float deltaTime);
		void Update(Animator& animator, float deltaTime);
		void OldRender(const NFGE::Graphics::Camera& camera);
		void Render(const NFGE::Graphics::Camera& camera);
		void DebugUI(const NFGE::Graphics::Camera& camera);

		bool SaveToFile(FILE* file, NFGE::Core::IOOption option = NFGE::Core::IOOption::Binary) const;
	private:
		bool LoadFromFile(FILE* file, NFGE::Core::IOOption option = NFGE::Core::IOOption::Binary);
		void RenderSkeleton(Bone* rootBone, const Camera& camera);
	};
}

#endif // !INCLUDE_NFGE_GRAGHICS_SKELETONMODEL_H
