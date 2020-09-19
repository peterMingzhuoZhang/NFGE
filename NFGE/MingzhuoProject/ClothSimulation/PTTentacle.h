#pragma once

#include "NFGE/Inc/NFGE.h"

using namespace NFGE::Graphics;
using namespace NFGE::Math;


// - SelfDefine type --------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
// ---vvvvvvvvvvvvvv---------------------------------------------------------------------------------------------------
namespace PC	//Peter chan style
{
	struct TentacleSegment
	{
		Bone mBone;
		Matrix4 boneMatrix;
		Vector3		mChildSpwanPoint;
		Vector3		mAnitmatePosition;
		Quaternion	mAnitmateRotation;

		std::vector<std::unique_ptr<Animation>> mAnimations;
		AnimationBuilder mAnimationBuilder;

		TentacleSegment(const Bone& bone, const Matrix4& boneMat, const Vector3& childSpwanPoint, const Vector3& animaitonPos, const Quaternion& animationRot)
			: mBone(bone)
			, boneMatrix(boneMat)
			, mChildSpwanPoint(childSpwanPoint)
			, mAnitmatePosition(animaitonPos)
			, mAnitmateRotation(animationRot)
		{}
		TentacleSegment(const TentacleSegment& other) {};
	};
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
		float padding[3];
	};
	using TransformBuffer = NFGE::Graphics::TypedConstantBuffer<TransformData>;
	using MaterialBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Graphics::Material>;
	using BoneTransformBuffer = NFGE::Graphics::TypedConstantBuffer<NFGE::Math::Matrix4[128]>;
	using OptionBuffer = NFGE::Graphics::TypedConstantBuffer<OptionsData>;
}
// ---^^^^^^^^^^^^^^---------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
// ---||||||||||||||---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


class PTTentacle	//Peter-Amy Tentacle: [Amy helps me create this one]
{
public:
	void Load(int totalSeg, float baseLength, const char* diffuseMap, const char* nomralMap, const char* specularMap, const char* displaceMentMap);
	void InitPhysics(NFGE::Physics::PhysicsWorld& world);
	void Update(float deltaTime);
	void Render(const Camera& camera);
	void DebugUI();
	void UnLoad();

	void SetPosition(const NFGE::Math::Vector3& newPos) { mPosition = newPos; };
	NFGE::Math::Vector3 GetPosition() const { return mPosition; };
private:
	void BuildBoneSkelton(int totleBone, float baseLength);
	void UpdateSegmentTransform(int index);
	void UpdateSegmentTransform_FromPhysics();
	void AddSupport(NFGE::Physics::PhysicsWorld& world, Vector3 displacement);
private:

	//-------------------------------------------------------------------------------------------
	static bool IsMeshInit;
	static NFGE::Graphics::BoneMesh mMesh;
	static NFGE::Graphics::MeshBuffer mMeshBuffer;

	NFGE::Math::Matrix4 mMatrices[3];
	NFGE::Graphics::Material mMaterial;
	PC::TransformBuffer mTransformBuffer;
	PC::MaterialBuffer mMaterialBuffer;
	PC::BoneTransformBuffer mBoneTransferBuffer;
	PC::OptionBuffer mOptionBuffer;

	NFGE::Graphics::VertexShader mVertexShader;
	NFGE::Graphics::PixelShader mPixelShader;

	NFGE::Math::Vector3 mPosition;
	NFGE::Math::Quaternion mRotation;
	NFGE::Math::Vector3 mScale;

	TextureId mDiffuseTexture;
	TextureId mSpecularTexture;
	TextureId mNormalTexture;
	TextureId mDisplacementTexture;
	float mBumpWeight = 0.0f;
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	int mTotalSeg = 3;
	std::vector<PC::TentacleSegment> mSegs;
	//-------------------------------------------------------------------------------------------
	//- Control ---------------------------------------------------------------------------------
	int mCurrentControlSeg = 0;
	//- Debug -----------------------------------------------------------------------------------
	int mCurrentControlVertex = 0;
	//-------------------------------------------------------------------------------------------

	//- Physics ---------------------------------------------------------------------------------
	std::vector<NFGE::Physics::Particle*> mGrassParticles;
	NFGE::Physics::Particle* mHookParticle;
	NFGE::Physics::Fixed* mRootFix;
	NFGE::Physics::Fixed* mHookFix;

};
