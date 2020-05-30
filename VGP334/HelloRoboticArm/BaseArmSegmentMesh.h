#ifndef INCLUDED_HELLOROBOTICARM_BASEARMSEGMENT_H
#define INCLUDED_HELLOROBOTICARM_BASEARMSEGMENT_H

#include "PTCube.h"

using namespace NFGE::Math;

class RobotBoneMesh
{
public:
	RobotBoneMesh(PTCube& geometry);
	RobotBoneMesh(PTCube& theGeometry, NFGE::Math::Vector3 adjestPosition, NFGE::Math::Quaternion adjestRotation, NFGE::Math::Vector3 adjestScale, NFGE::Math::Vector4 ambientColor, NFGE::Math::Vector4 diffuseColor);
	RobotBoneMesh(PTCube& theGeometry, const NFGE::Math::Vector4 ambient, const NFGE::Math::Vector4 diffuse, const NFGE::Math::Vector3 childSpawnPos, const NFGE::Math::Matrix4& mAdjestMat);
		
	RobotBoneMesh(const RobotBoneMesh& other) = default;
	RobotBoneMesh& operator=(const RobotBoneMesh& other) = default;

	void Load(){}
	void Update(float deltaTime){}
	void Render(const NFGE::Math::Matrix4& toWorldMat, const NFGE::Graphics::Camera& camera, bool isSelected);
	void UnLoad() {}

	// Setter & Getter
	NFGE::Math::Matrix4 GetAdjestMatrix() const {return mAdjextMatrix;}

	NFGE::Math::Vector3 GetChildSpwanPosition() const { return mChildSpwanPosition; }

	void SetDiffuseColor(const NFGE::Math::Vector4& color) { mDiffuseColor = color; }
	NFGE::Math::Vector4 GetDiffuseColor() const { return mDiffuseColor; }

	void SetAmbientColor(const NFGE::Math::Vector4& color) { mAmbientColor = color; }
	NFGE::Math::Vector4 GetAmbientColor() const { return mAmbientColor; }

	// Util
	void LoadFromFile(FILE* inputFile)
	{
		fscanf(inputFile, "ambient: %f %f %f %f\n", &mAmbientColor.x, &mAmbientColor.y, &mAmbientColor.z, &mAmbientColor.w);
		fscanf(inputFile, "diffuse: %f %f %f %f\n", &mDiffuseColor.x, &mDiffuseColor.y, &mDiffuseColor.z, &mDiffuseColor.w);
		fscanf(inputFile, "childSpwanPos: %f %f %f\n", &mChildSpwanPosition.x, &mChildSpwanPosition.y, &mChildSpwanPosition.z);
		fscanf(inputFile, "adjestMat: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", 
			&mAdjextMatrix._11 , &mAdjextMatrix._12, &mAdjextMatrix._13, &mAdjextMatrix._14,
			&mAdjextMatrix._21, &mAdjextMatrix._22, &mAdjextMatrix._23, &mAdjextMatrix._24,
			&mAdjextMatrix._31, &mAdjextMatrix._32, &mAdjextMatrix._33, &mAdjextMatrix._34,
			&mAdjextMatrix._41, &mAdjextMatrix._42, &mAdjextMatrix._43, &mAdjextMatrix._44
			);
	}
	void SaveToFile(FILE* outputFile)
	{
		fprintf(outputFile, "ambient: %f %f %f %f\n", mAmbientColor.x, mAmbientColor.y, mAmbientColor.z, mAmbientColor.w);
		fprintf(outputFile, "diffuse: %f %f %f %f\n", mDiffuseColor.x, mDiffuseColor.y, mDiffuseColor.z, mDiffuseColor.w);
		fprintf(outputFile, "childSpwanPos: %f %f %f\n", mChildSpwanPosition.x, mChildSpwanPosition.y, mChildSpwanPosition.z);
		fprintf(outputFile, "adjestMat: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			mAdjextMatrix._11, mAdjextMatrix._12, mAdjextMatrix._13, mAdjextMatrix._14,
			mAdjextMatrix._21, mAdjextMatrix._22, mAdjextMatrix._23, mAdjextMatrix._24,
			mAdjextMatrix._31, mAdjextMatrix._32, mAdjextMatrix._33, mAdjextMatrix._34,
			mAdjextMatrix._41, mAdjextMatrix._42, mAdjextMatrix._43, mAdjextMatrix._44
		);
	}
private:
	PTCube& mGeometry;

	NFGE::Math::Vector4		mAmbientColor;
	NFGE::Math::Vector4		mDiffuseColor;

	NFGE::Math::Vector3		mChildSpwanPosition;
	NFGE::Math::Matrix4		mAdjextMatrix;

};

#endif // !INCLUDED_HELLOROBOTICARM_BASEARMSEGMENT_H
