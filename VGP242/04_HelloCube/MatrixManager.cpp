#include "MatrixManager.h"
#include"MingzhuoUtility.h"

//====================================================================================================
// Statics
//====================================================================================================

MatrixManager* MatrixManager::s_pInstance = NULL;

//====================================================================================================
// Class
//====================================================================================================

//====================================================================================================
// Function Definitions
//====================================================================================================

void MatrixManager::Add3DTranslation(float tx, float ty, float tz)
{
	if (false == m_Current3DMatrix.IsIdentity())
	{
		Matrix4 newTransform;
		newTransform.Translation(tx, ty, tz);

		m_Current3DMatrix = m_Current3DMatrix * newTransform ;
	}
	else
	{
		m_Current3DMatrix.Translation(tx, ty, tz);
	}
}

void MatrixManager::Add3DScaling(float sx, float sy, float sz)
{
	if (false == m_Current3DMatrix.IsIdentity())
	{
		Matrix4 newTransform;
		newTransform.Scaling(Vector4(sx, sy, sz, 1.0f));

		m_Current3DMatrix = m_Current3DMatrix * newTransform;
	}
	else
	{
		m_Current3DMatrix.Scaling(Vector4(sx, sy, sz, 1));
	}
}

void MatrixManager::Add3DRotation(float rx, float ry, float rz)
{
	// 
	if (false == m_Current3DMatrix.IsIdentity())
	{
		Matrix4 newTransform;
		newTransform.Rotation(rx,ry,rz);

		m_Current3DMatrix = m_Current3DMatrix * newTransform;
	}
	else
	{
		m_Current3DMatrix.Rotation(rx, ry, rz);
	}
}

void MatrixManager::Add3DShearing(float shx, float shy, float shz)
{
	if (false == m_Current3DMatrix.IsIdentity())
	{
		Matrix4 newTransform;
		newTransform.Shear(shx, shy, shz);

		m_Current3DMatrix = m_Current3DMatrix * newTransform;
	}
	else
	{
		m_Current3DMatrix.Shear(shx, shy, shz);
	}
}

void MatrixManager::UpdateCurrent3DTransformWith_MingzhuoUitility()
{
	//if (false == m_Current3DMatrix.IsIdentity())
	{
		Transform3D theTransform = MingzhuoUtility::GetInstance()->Geo3DTransform;
		Matrix4 newTransform_Translate;
		newTransform_Translate.Translation(theTransform.tX, theTransform.tY, theTransform.tZ);

		Matrix4 newTransform_Scale;
		Vector4 scale(theTransform.scX, theTransform.scY, theTransform.scZ, 1.0f);
		newTransform_Scale.Scaling3D(scale);

		Matrix4 newTransform_rotation;
		newTransform_rotation.Rotation(theTransform.rX, theTransform.rY, theTransform.rZ);

		Matrix4 newTransform_Shear;
		newTransform_Shear.Shear(theTransform.shX, theTransform.shX, theTransform.shX);

		m_Current3DMatrix = m_Current3DMatrix * newTransform_Scale * newTransform_Shear * newTransform_rotation * newTransform_Translate;
	}
}

MatrixManager::MatrixManager()
{
	m_Current3DMatrix.Identity();
}

//----------------------------------------------------------------------------------------------------

MatrixManager* MatrixManager::GetInstance( void )
{
    if ( NULL == s_pInstance )
    {
        s_pInstance = new MatrixManager();
    }
    return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

const Matrix4 & MatrixManager::GetCurrent3DTransform(void) const
{
	return m_Current3DMatrix;
}

//----------------------------------------------------------------------------------------------------

MatrixManager::~MatrixManager( void )
{
    // empty destructor
}