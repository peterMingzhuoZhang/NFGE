#ifndef HELLOCUBE_MATRIXMANAGER_H
#define HELLOCUBE_MATRIXMANAGER_H

#include <NFGEMath/Inc/NFGEMath.h>

using namespace NFGE::Math;

class MatrixManager
{
public:
	~MatrixManager(void);

	// Instance Retrieval
	static MatrixManager* GetInstance(void);

	// function to retrieve the currently set matrix transform
	const Matrix4& GetCurrent3DTransform(void) const;

	// Matrix3D state functions
	void Set3DIdentity(void) { m_Current3DMatrix.Identity(); }
	void Add3DTranslation(float tx, float ty, float tz);
	void Add3DScaling(float sx, float sy, float sz);
	void Add3DRotation(float rx, float ry, float rz);
	void Add3DShearing(float shx, float shy, float shz);

	void UpdateCurrent3DTransformWith_MingzhuoUitility();

protected:
	// Protected Operations
	MatrixManager();
	MatrixManager(const MatrixManager&) = delete;
	MatrixManager& operator=(const MatrixManager&) = delete;

private:
	// static singleton instance
	static MatrixManager*    s_pInstance;

	// members
	Matrix4   m_Current3DMatrix;

};

#endif // !HELLOCUBE_MATRIXMANAGER_H

