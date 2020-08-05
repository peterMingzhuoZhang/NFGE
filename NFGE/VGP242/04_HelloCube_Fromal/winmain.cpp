#include <Core\Inc\Core.h>
#include <Graphics\Inc\Graphics.h>
#include <NFGEMath/Inc/NFGEMath.h>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

void CameraControl(Camera& camera);
void CubeTransfromControl(Transform3D& transfrom);
Matrix4 GetTransformMatrix(const Transform3D& theTransform);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup our application window
	int windowWidth = 1280;
	int windowHeight = 720;

	Window myWindow;

	myWindow.Initialize(hInstance, "Hello Shapes", windowWidth, windowHeight);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	std::vector<MeshBuffer> myMeshes;

	VertexPC cubeVertices[] =
	{
		{ { -0.1f,  0.1f, 0.5f }, {1.0f, 0.0f, 0.0f, 1.0f }},		//0	//{ 0,1,2 }
		{ {  0.1f,  0.1f, 0.5f }, {1.0f, 0.0f, 0.0f, 1.0f }},		//1
		{ { -0.1f, -0.1f, 0.5f }, {0.0f, 1.0f, 0.0f, 1.0f }},		//2
		{ {  0.1f, -0.1f, 0.5f }, {0.0f, 1.0f, 0.0f, 1.0f }},		//3
														  
		{ { -0.1f,  0.1f, 0.7f }, {0.0f, 0.0f, 1.0f, 1.0f }},		//4	//{ 0,1,2 }
		{ {  0.1f,  0.1f, 0.7f }, {0.0f, 0.0f, 1.0f, 1.0f }},		//5
		{ { -0.1f, -0.1f, 0.7f }, {1.0f, 1.0f, 0.0f, 1.0f }},		//6
		{ {  0.1f, -0.1f, 0.7f }, {1.0f, 1.0f, 0.0f, 1.0f }},		//7

	};

	VertexPC cube2Vertices[] =
	{
		{ { -1.0f,  1.0f, -1.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },	//0	//{ 0,1,2 }
		{ {  1.0f,  1.0f, -1.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },	//1
		{ { -1.0f, -1.0f, -1.0f }, {0.0f, 1.0f, 0.0f, 1.0f} },	//2
		{ {  1.0f, -1.0f, -1.0f }, {0.0f, 1.0f, 0.0f, 1.0f} },	//3

		{ { -1.0f,  1.0f, 1.0f }, {0.0f, 0.0f, 1.0f, 1.0f }},		//4	//{ 0,1,2 }
		{ {  1.0f,  1.0f, 1.0f }, {0.0f, 0.0f, 1.0f, 1.0f }},		//5
		{ { -1.0f, -1.0f, 1.0f }, {1.0f, 1.0f, 0.0f, 1.0f }},		//6
		{ {  1.0f, -1.0f, 1.0f }, {1.0f, 1.0f, 0.0f, 1.0f }},		//7

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

	Transform3D CubeTransfromInfo{};

	MeshBuffer cubeMesh{};
	cubeMesh.Initialize(cube2Vertices, static_cast<uint32_t>(std::size(cube2Vertices)), sizeof(VertexPC), cubeIndexBuffer, static_cast<uint32_t>(std::size(cubeIndexBuffer)));
	myMeshes.push_back(cubeMesh);

	VertexShader vertexShader{};
	vertexShader.Initialize(L"../../Assets/Shaders/DoTransform.fx", VertexPC::Format);

	PixelShader pixelShader{};
	pixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");

	ConstantBuffer constantBuffer;
	constantBuffer.Initialize(3 * sizeof(Matrix4));


	Camera camera;
	camera.SetPosition({0.0f, 0.0f, -10.0f});
	camera.SetDirection({ 0.0f, 0.0f, 1.0f });




	bool done = false;

	while (!done)
	{
		size_t currentMesh = 0;
		size_t totalMesh = myMeshes.size();
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		else if (GetAsyncKeyState(VK_NUMPAD1))
		{
			currentMesh++;
			currentMesh %= totalMesh;
		}
		
		CameraControl(camera);
		CubeTransfromControl(CubeTransfromInfo);

		// Run your game logic ...
		GraphicsSystem::Get()->BeginRender();

		vertexShader.Bind();
		pixelShader.Bind();

		Matrix4 matrices[3];
		Matrix4 idmat{};
		idmat.Identity();
		matrices[0] = Transpose(GetTransformMatrix(CubeTransfromInfo));
		matrices[1] = Transpose(camera.GetViewMatrix());
		matrices[2] = Transpose(camera.GetPerspectiveMatrix(static_cast<float>(windowWidth) / static_cast<float>(windowHeight)));
		
		constantBuffer.Load(matrices);
		constantBuffer.BindVS();

		myMeshes[currentMesh].Render();

		GraphicsSystem::Get()->EndRender();
	}

	vertexShader.Terminate();
	pixelShader.Terminate();
	constantBuffer.Terminate();
	for (auto& mesh : myMeshes)
	{
		mesh.Terminate();
	}

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();


	return 0;
}

void CameraControl(Camera& camera)
{
	float cameraSpeed = 0.1f;
	if (GetAsyncKeyState(VK_UP))
		{
		camera.Walk(cameraSpeed);
		}
	else if (GetAsyncKeyState(VK_DOWN))
		{
		camera.Walk(-cameraSpeed);
		}
	else if (GetAsyncKeyState(VK_LEFT))
		{
		camera.Strafe(-cameraSpeed);
		}
	else if (GetAsyncKeyState(VK_RIGHT))
		{
		camera.Strafe(cameraSpeed);
		}

	if (GetAsyncKeyState(VK_NUMPAD8))
	{
		camera.Pitch(-cameraSpeed);
	}
	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		camera.Pitch(cameraSpeed);
	}
	if (GetAsyncKeyState(VK_NUMPAD4))
	{
		camera.Yaw(-cameraSpeed);
		
	}
	if (GetAsyncKeyState(VK_NUMPAD6))
	{
		camera.Yaw(cameraSpeed);
		
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		camera.Rise(cameraSpeed);
	}
	if (GetAsyncKeyState(VK_CONTROL))
	{
		camera.Rise(-cameraSpeed);
	}
}

void CubeTransfromControl(Transform3D& transfrom)
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
}

Matrix4 GetTransformMatrix(const Transform3D& theTransform)
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