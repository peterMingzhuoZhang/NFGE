#include "Plante.h"

const wchar_t* shaderFileName = L"../../Assets/Shaders/Texturing.fx";
const wchar_t* shaderFileName2 = L"../../Assets/Shaders/DoTransform.fx";

Matrix4 Planet::GetTransformMatrix(const Transform3D & theTransform)
{
	
	Matrix4 ret{};
	ret.Identity();
	Matrix4 newTransform_Translate;
	newTransform_Translate.Translation(theTransform.tX, theTransform.tY, theTransform.tZ);

	Matrix4 newTransform_Scale;
	Vector4 scale(theTransform.scX, theTransform.scY, theTransform.scZ, 1.0f);
	newTransform_Scale.Scaling3D(scale);

	Matrix4 newTransform_rotation;
	newTransform_rotation.Rotation(theTransform.rX, theTransform.rY, theTransform.rZ);

	Matrix4 newTransform_Shear;
	newTransform_Shear.Shear(theTransform.shX, theTransform.shY, theTransform.shZ);

	ret = ret * newTransform_Scale * newTransform_Shear * newTransform_rotation * newTransform_Translate;

	return ret;

	
}

void Planet::TransfromControl(Transform3D & transfrom)
{
	
		// Rotate
		float changeSpeed = 1.0f;
		if (GetAsyncKeyState('Q'))
		{
			transfrom.rX += changeSpeed;
		}
		if (GetAsyncKeyState('W'))
		{
			transfrom.rY += changeSpeed;
		}
		if (GetAsyncKeyState('E'))
		{
			transfrom.rZ += changeSpeed;
		}
		// Scale
		if (GetAsyncKeyState('A'))
		{
			transfrom.scX += changeSpeed * 0.1f;
			transfrom.scY += changeSpeed * 0.1f;
			transfrom.scZ += changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('S'))
		{
			transfrom.scY += changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('D'))
		{
			transfrom.scZ += changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('Z'))
		{
			transfrom.scX -= changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('X'))
		{
			transfrom.scY -= changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('C'))
		{
			transfrom.scZ -= changeSpeed * 0.1f;
		}
		// Shear
		if (GetAsyncKeyState('U'))
		{
			transfrom.shX += changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('I'))
		{
			transfrom.shY += changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('O'))
		{
			transfrom.shZ += changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('J'))
		{
			transfrom.shX -= changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('K'))
		{
			transfrom.shY -= changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('L'))
		{
			transfrom.shZ -= changeSpeed * 0.1f;
		}
		if (GetAsyncKeyState('M'))
		{
			transfrom.tX += changeSpeed * 0.1f;
		}
	
		if (GetAsyncKeyState('R'))		// Reset
		{
			transfrom = Transform3D{};
		}
}

void Planet::Load(const MeshPX& mesh, const wchar_t * textureName)
{
	mMatrices[0].Identity();
	mMatrices[1].Identity();
	mMatrices[2].Identity();
	mMeshBuffer.Initialize(mesh);
	mMeshBuffer.SetTopology();
	mVertexShader.Initialize(shaderFileName, VertexPX::Format);
	mPixelShader.Initialize(shaderFileName);
	mConstantBuffer.Initialize(3 * sizeof(Matrix4));
	mTexture.Initialize(textureName);
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void Planet::Load(const MeshPC& mesh)
{
	mMatrices[0].Identity();
	mMatrices[1].Identity();
	mMatrices[2].Identity();
	mMeshBuffer.Initialize(mesh);
	mMeshBuffer.SetTopology();
	mVertexShader.Initialize(shaderFileName2, VertexPC::Format);
	mPixelShader.Initialize(shaderFileName2);
	mConstantBuffer.Initialize(3 * sizeof(Matrix4));
	mTexture.Initialize(L"../Assets/Images/basketball.jpg");
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void Planet::Unload()
{
	mMeshBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
}

void Planet::Update(PlanetMovment movment)
{
	Transform3D selfRot{};
	selfRot.rX = movment.SelfRotate.x; selfRot.rY = movment.SelfRotate.y; selfRot.rZ = movment.SelfRotate.z;

	Transform3D MoveToSroundTarget{};
	MoveToSroundTarget.tX = movment.SroundTarget.x; MoveToSroundTarget.tY = movment.SroundTarget.y; MoveToSroundTarget.tZ = movment.SroundTarget.z;

	Transform3D SroundRotation{};
	SroundRotation.rX = movment.SroundRotate.x; SroundRotation.rY = movment.SroundRotate.y; SroundRotation.rZ = movment.SroundRotate.z;

	Transform3D MoveAwayFromTarget{};
	MoveAwayFromTarget.tX = movment.SroundDistance.x; MoveAwayFromTarget.tY = movment.SroundDistance.y; MoveAwayFromTarget.tZ = movment.SroundDistance.z;

	mMatrices[0].Identity();
	//mMatrices[0] = GetTransformMatrix(MoveToSroundTarget) * GetTransformMatrix(SroundRotation) * GetTransformMatrix(MoveAwayFromTarget)  * GetTransformMatrix(selfRot) *  GetTransformMatrix(mTransfromInfo)* mMatrices[0];
	mMatrices[0] = mMatrices[0] * GetTransformMatrix(mInitTransfromInfo)* GetTransformMatrix(mTransfromInfo) *GetTransformMatrix(selfRot) *GetTransformMatrix(MoveAwayFromTarget)  *GetTransformMatrix(SroundRotation) * GetTransformMatrix(MoveToSroundTarget);

	mPosition = Vector3{} * mMatrices[0];

	//mTransfromInfo = Transform3D();
}

void Planet::Render(const Camera& camera, int windowWidth, int windowHeight)
{

	mVertexShader.Bind();
	mMatrices[0] = Transpose(mMatrices[0]);
	mMatrices[1] = Transpose(camera.GetViewMatrix());
	mMatrices[2] = Transpose(camera.GetPerspectiveMatrix(static_cast<float>(windowWidth) / static_cast<float>(windowHeight)));
	mConstantBuffer.Load(mMatrices);
	mConstantBuffer.BindVS();

	mPixelShader.Bind();
	mTexture.BindPS();
	mSampler.BindPS();

	mMeshBuffer.Render();

}
