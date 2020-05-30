#include "BaseArmSegmentMesh.h"

RobotBoneMesh::RobotBoneMesh(PTCube & geometry)
	: mGeometry(geometry)
{}

RobotBoneMesh::RobotBoneMesh(PTCube & theGeometry, NFGE::Math::Vector3 adjestPosition, NFGE::Math::Quaternion adjestRotation, NFGE::Math::Vector3 adjestScale, NFGE::Math::Vector4 ambientColor, NFGE::Math::Vector4 diffuseColor)
	: mGeometry(theGeometry)
	, mAmbientColor(ambientColor)
	, mDiffuseColor(diffuseColor)
{
	NFGE::Math::Matrix4 mat = NFGE::Math::MatrixRotationQuaternion(adjestRotation);
	mat._11 *= adjestScale.x;
	mat._22 *= adjestScale.y;
	mat._33 *= adjestScale.z;
	mat._41 = adjestPosition.x;
	mat._42 = adjestPosition.y;
	mat._43 = adjestPosition.z;
	mAdjextMatrix = mat;

	mChildSpwanPosition = Vector3{ 0.0f,1.0f,0.0f } *mAdjextMatrix;
}

RobotBoneMesh::RobotBoneMesh(PTCube & theGeometry, const NFGE::Math::Vector4 ambient, const NFGE::Math::Vector4 diffuse, const NFGE::Math::Vector3 childSpawnPos, const NFGE::Math::Matrix4 & mAdjestMat)
	: mGeometry(theGeometry)
	, mAmbientColor(ambient)
	, mDiffuseColor(diffuse)
	, mChildSpwanPosition(childSpawnPos)
	, mAdjextMatrix(mAdjestMat)
{
}

void RobotBoneMesh::Render(const NFGE::Math::Matrix4 & toWorldMat, const NFGE::Graphics::Camera & camera, bool isSelected)
{
	mGeometry.Render(GetAdjestMatrix(), toWorldMat, camera, isSelected ? NFGE::Graphics::Colors::White : mDiffuseColor, mAmbientColor);
}
