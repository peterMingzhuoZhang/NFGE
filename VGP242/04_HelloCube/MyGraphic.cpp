#include "MyGraphic.h"
#include "MingzhuoUtility.h"

VertexPC cubeVertices[] =
{
	{ { -0.1f,  0.1f, 0.5f }, {1.0f, 0.0f, 0.0f, 1.0f }},	//0	//{ 0,1,2 }
	{ {  0.1f,  0.1f, 0.5f }, {1.0f, 0.0f, 0.0f, 1.0f }},	//1
	{ { -0.1f, -0.1f, 0.5f }, {0.0f, 1.0f, 0.0f, 1.0f }},	//2
	{ {  0.1f, -0.1f, 0.5f }, {0.0f, 1.0f, 0.0f, 1.0f }},	//3

	{ { -0.1f,  0.1f, 0.7f }, {0.0f, 0.0f, 1.0f, 1.0f }},	//4	//{ 0,1,2 }
	{ {  0.1f,  0.1f, 0.7f }, {0.0f, 0.0f, 1.0f, 1.0f }},	//5
	{ { -0.1f, -0.1f, 0.7f }, {1.0f, 1.0f, 0.0f, 1.0f }},	//6
	{ {  0.1f, -0.1f, 0.7f }, {1.0f, 1.0f, 0.0f, 1.0f }},	//7

};

uint32_t cubeIndexBuffer[] =
{
	0,1,2,//|
	1,3,2,//|--- Front face

	1,7,3,//|
	1,5,7,//|--- Right face

	5,4,6,//|
	5,6,7,//|--- Back face

	4,0,2,//|
	4,2,6,//|--- Left face

	4,1,0,//|
	4,5,1,//|--- Top face

	3,7,6,//|
	2,3,6 //|--- Bottom face
};

MyGraphic::MyGraphic()
{
	
}

MyGraphic::~MyGraphic()
{
	
}

void MyGraphic::Initialize(Window* theWindow, int theWindowWidth, int theWindowHeight)
{
	mWindow = theWindow;
	mWindowWidth = theWindowWidth;
	mWindowHeight = theWindowHeight;
	GraphicsSystem::StaticInitialize(mWindow->GetWindowHandle(), false);
	ratio = static_cast<float>(mWindowHeight) / static_cast<float>(mWindowWidth);
	LoadContext();
	vertexShader.Initialize(L"../../Assets/Shaders/DoTransform.fx", VertexPC::Format);
	pixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");

}

void MyGraphic::Terminate()
{
	vertexShader.Terminate();
	pixelShader.Terminate();
	for (auto& mesh : myMeshes)
	{
		mesh.Terminate();
	}

	GraphicsSystem::StaticTerminate();

	mWindow->Terminate();
}

void MyGraphic::Update()
{
	//Do transformation here
	ASSERT(currentMesh < myMeshes.size(), "meshindex out of bound.");

	uint32_t verticesCount = myCubeContexts[currentMesh].mVerticesCount;
	VertexPC* oriVertices = myCubeContexts[currentMesh].mVertices;
	VertexPC* newVertices = new VertexPC[verticesCount];

	MingzhuoUtility::GetInstance()->Update();

	// Set Camera context
	PTCamera::GetInstance()->SetCamera(0.0f, 0.0f, -10.0f, 0.0f, 0.0f, 20.0f);
	PTCamera::GetInstance()->SetFrustum(90.0f);
	PTCamera::GetInstance()->SetProjection_Matrix();
	PTCamera::GetInstance()->SetConverting_Matrixs();

	Geometry3DTransform(oriVertices, newVertices, verticesCount);
	
	myMeshes[currentMesh].Initialize(newVertices, myCubeContexts[currentMesh].mVerticesCount, sizeof(VertexPC), myCubeContexts[currentMesh].mIndexBuffer, myCubeContexts[currentMesh].mIndexCount);

}

void MyGraphic::Render()
{
	// Run your game logic ...
	GraphicsSystem::Get()->BeginRender();

	vertexShader.Bind();
	pixelShader.Bind();
	myMeshes[currentMesh].Render();

	GraphicsSystem::Get()->EndRender();
}

void MyGraphic::LoadContext()
{
	// Set cube context
	PTContext cubeContext;
	cubeContext.mVertices = cubeVertices;
	cubeContext.mVerticesCount = static_cast<uint32_t>(std::size(cubeVertices));
	cubeContext.mIndexBuffer = cubeIndexBuffer;
	cubeContext.mIndexCount = static_cast<uint32_t>(std::size(cubeIndexBuffer));

	myCubeContexts.push_back(cubeContext);

	MeshBuffer cubeMesh{};
	myMeshes.push_back(cubeMesh);

	MatrixManager::GetInstance()->Add3DRotation(10.0f,20.0f,0.0f);
	
}

void MyGraphic::Geometry3DTransform(VertexPC* theVertices, VertexPC* newVertices, uint32_t size)
{
	// number of vertices in vertex buffer
	unsigned int uiNumVertices = size;

	// current primitive and fill states

	// Apply the current matrix transform to all of the vertices
	MatrixManager::GetInstance()->UpdateCurrent3DTransformWith_MingzhuoUitility();
	Matrix4 currentTransform = MatrixManager::GetInstance()->GetCurrent3DTransform();
	
	Matrix4 worldToView_Matrix = PTCamera::GetInstance()->GetWorldToView_Matrix();
	Matrix4 viewToNDC = PTCamera::GetInstance()->GetProjection_Matrix();

	// Transform base on camera
	//Vector4 cameraOri = Camera::GetInstance()->GetOrigin();
	//Matrix4 toCameraOri; toCameraOri.Identity(); toCameraOri.mV[3] = -cameraOri.x; toCameraOri.mV[7] = -cameraOri.y; toCameraOri.mV[11] = -cameraOri.z;
	//Matrix4 backFromCameraOri; backFromCameraOri.Identity(); backFromCameraOri.mV[3] = cameraOri.x; backFromCameraOri.mV[7] = cameraOri.y; backFromCameraOri.mV[11] = cameraOri.z;

	//Matrix4 CombineMatrix = worldToView_Matrix * currentTransform;

	Vector4 currentVector;
	currentVector.w = 1;        // homogeneous coordinate
	for (unsigned int i = 0; i < uiNumVertices; ++i)
	{
		// transfer point values to a CVector3 for Matrix Calculations
		currentVector.x = theVertices[i].position.x;
		currentVector.y = theVertices[i].position.y;
		currentVector.z = theVertices[i].position.z;

		//Matrix4 tOri; tOri.Identity(); tOri.mV[3] = cameraOri.x - currentVector.x; tOri.mV[7] = cameraOri.y - currentVector.y; tOri.mV[11] = cameraOri.z - currentVector.z;
		//Matrix4 backFromOri; backFromOri.Identity(); backFromOri.mV[3] = currentVector.x - cameraOri.x; backFromOri.mV[7] = currentVector.y - cameraOri.y; backFromOri.mV[11] = currentVector.z - cameraOri.z;
		// perform transform
		Matrix4 CombineMatrix = currentTransform * worldToView_Matrix ;
		currentVector = currentVector * CombineMatrix ;
		currentVector.WTo1();

		currentVector = currentVector * viewToNDC;
		currentVector.WTo1();

		VertexPC newVertex{ {currentVector.x,currentVector.y,currentVector.z},{theVertices[i].color.r,theVertices[i].color.g,theVertices[i].color.b,theVertices[i].color.a} };

		newVertices[i] = newVertex;
	}

}