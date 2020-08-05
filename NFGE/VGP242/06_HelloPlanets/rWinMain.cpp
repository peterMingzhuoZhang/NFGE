//#include <Core\Inc\Core.h>
//#include <Graphics\Inc\Graphics.h>
//#include <NFGEMath/Inc/NFGEMath.h>
//
//using namespace NFGE::Core;
//using namespace NFGE::Graphics;
//using namespace NFGE::Math;
//
//const wchar_t* shaderFileName = L"../Assets/Shaders/Texturing.fx";
//
//void CameraControl(Camera& camera);
//void CubeTransfromControl(Transform3D& transfrom);
//Matrix4 GetTransformMatrix(const Transform3D& theTransform);
//
//int WINAPI WinMain2(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
//{
//	// Setup our application window
//	int windowWidth = 1280;
//	int windowHeight = 720;
//
//	Window myWindow;
//
//	myWindow.Initialize(hInstance, "Hello Shapes", windowWidth, windowHeight);
//
//	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
//	std::vector<MeshBuffer> myMeshes;
//
//	MeshPX cube = MeshBuilder::CreateCubePX();
//	MeshPX plane = MeshBuilder::CreatePlanePX();
//	MeshPC cylinder = MeshBuilder::CreateCylinderPC(4, 20, 3.0f, 5.0f);
//	MeshPC cone = MeshBuilder::CreateConePC(4, 20, 3.0f, 5.0f);
//	MeshPC sphere = MeshBuilder::CreateSpherePC(10, 20, 3.0f);
//	MeshPC torus = MeshBuilder::CreateTorusPC(10, 10, 5.0f, 7.0f);
//
//	Transform3D CubeTransfromInfo{};
//
//	// ----------------------------------------------
//	// --------- Initlizing MeshBuffers -------------
//	MeshBuffer cubeMesh{};
//	cubeMesh.Initialize(cube);
//	myMeshes.push_back(cubeMesh);
//
//	MeshBuffer planeBuffer{};
//	planeBuffer.Initialize(plane);
//	myMeshes.push_back(planeBuffer);
//	//
//	//MeshBuffer cylinderBuffer{};
//	//cylinderBuffer.Initialize(cylinder);
//	//myMeshes.push_back(cylinderBuffer);
//	//
//	//MeshBuffer coneBuffer{};
//	//coneBuffer.Initialize(cone);
//	//myMeshes.push_back(coneBuffer);
//	//
//	//MeshBuffer sphereBuffer{};
//	//sphereBuffer.Initialize(sphere);
//	//myMeshes.push_back(sphereBuffer);
//	//
//	//MeshBuffer torusBuffer{};
//	//torusBuffer.Initialize(torus);
//	//myMeshes.push_back(torusBuffer);
//	// ----------------------------------------------
//
//	VertexShader vertexShader{};
//	vertexShader.Initialize(shaderFileName, VertexPX::Format);
//
//	PixelShader pixelShader{};
//	pixelShader.Initialize(shaderFileName);
//
//	ConstantBuffer constantBuffer;
//	constantBuffer.Initialize(sizeof(Matrix4));
//
//	Texture texture;
//	texture.Initialize(L"../Assets/Images/brickwork.jpg");
//
//	Sampler sampler;
//	sampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
//
//	Camera camera;
//	camera.SetPosition({ 0.0f, 0.0f, -10.0f });
//	camera.SetDirection({ 0.0f, 0.0f, 1.0f });
//
//
//
//
//	bool done = false;
//	size_t currentMesh = 0;
//
//	while (!done)
//	{
//		size_t totalMesh = myMeshes.size();
//		done = myWindow.ProcessMessage();
//		if (GetAsyncKeyState(VK_ESCAPE))
//		{
//			done = true;
//		}
//
//		if (GetAsyncKeyState(VK_ADD))
//		{
//			currentMesh++;
//			currentMesh %= totalMesh;
//		}
//
//		for (int i = 0; i < 10; i++)
//		{
//			if (GetAsyncKeyState('0' + i))
//			{
//				currentMesh = i % (myMeshes.size());
//			}
//		}
//
//
//		CameraControl(camera);
//		CubeTransfromControl(CubeTransfromInfo);
//
//		// Run your game logic ...
//		GraphicsSystem::Get()->BeginRender();
//
//		auto context = GraphicsSystem::Get()->GetContext();
//
//		vertexShader.Bind(context);
//
//		Matrix4 matrices[3];
//		Matrix4 idmat{};
//		idmat.Identity();
//		matrices[0] = Transpose(GetTransformMatrix(CubeTransfromInfo));
//		matrices[1] = Transpose(camera.GetViewMatrix());
//		matrices[2] = Transpose(camera.GetPerspeciveMatrix(static_cast<float>(windowWidth) / static_cast<float>(windowHeight)));
//		constantBuffer.Load(context, matrices);
//		constantBuffer.Bind(context);
//
//		pixelShader.Bind(context);
//		texture.BindPS();
//		sampler.BindPS();
//
//		myMeshes[currentMesh].Render();
//
//		GraphicsSystem::Get()->EndRender();
//	}
//
//	vertexShader.Terminate();
//	pixelShader.Terminate();
//	constantBuffer.Terminate();
//	for (auto& mesh : myMeshes)
//	{
//		mesh.Terminate();
//	}
//	texture.Terminate();
//	sampler.Terminate();
//
//	GraphicsSystem::StaticTerminate();
//
//	myWindow.Terminate();
//
//
//	return 0;
//}
//
//void CameraControl(Camera& camera)
//{
//	float cameraSpeed = 0.1f;
//	if (GetAsyncKeyState(VK_UP))
//	{
//		camera.Walk(cameraSpeed);
//	}
//	else if (GetAsyncKeyState(VK_DOWN))
//	{
//		camera.Walk(-cameraSpeed);
//	}
//	else if (GetAsyncKeyState(VK_LEFT))
//	{
//		camera.Strafe(-cameraSpeed);
//	}
//	else if (GetAsyncKeyState(VK_RIGHT))
//	{
//		camera.Strafe(cameraSpeed);
//	}
//
//	if (GetAsyncKeyState(VK_NUMPAD8))
//	{
//		camera.Pitch(-cameraSpeed);
//	}
//	if (GetAsyncKeyState(VK_NUMPAD2))
//	{
//		camera.Pitch(cameraSpeed);
//	}
//	if (GetAsyncKeyState(VK_NUMPAD4))
//	{
//		camera.Yaw(-cameraSpeed);
//
//	}
//	if (GetAsyncKeyState(VK_NUMPAD6))
//	{
//		camera.Yaw(cameraSpeed);
//
//	}
//
//	if (GetAsyncKeyState(VK_SPACE))
//	{
//		camera.Rise(cameraSpeed);
//	}
//	if (GetAsyncKeyState(VK_CONTROL))
//	{
//		camera.Rise(-cameraSpeed);
//	}
//}
//
//void CubeTransfromControl(Transform3D& transfrom)
//{
//	// Rotate
//	float changeSpeed = 1.0f;
//	if (GetAsyncKeyState('Q'))
//	{
//		transfrom.rX += changeSpeed;
//	}
//	if (GetAsyncKeyState('W'))
//	{
//		transfrom.rY += changeSpeed;
//	}
//	if (GetAsyncKeyState('E'))
//	{
//		transfrom.rZ += changeSpeed;
//	}
//	// Scale
//	if (GetAsyncKeyState('A'))
//	{
//		transfrom.scX += changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('S'))
//	{
//		transfrom.scY += changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('D'))
//	{
//		transfrom.scZ += changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('Z'))
//	{
//		transfrom.scX -= changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('X'))
//	{
//		transfrom.scY -= changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('C'))
//	{
//		transfrom.scZ -= changeSpeed * 0.1f;
//	}
//	// Shear
//	if (GetAsyncKeyState('U'))
//	{
//		transfrom.shX += changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('I'))
//	{
//		transfrom.shY += changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('O'))
//	{
//		transfrom.shZ += changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('J'))
//	{
//		transfrom.shX -= changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('K'))
//	{
//		transfrom.shY -= changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('L'))
//	{
//		transfrom.shZ -= changeSpeed * 0.1f;
//	}
//	if (GetAsyncKeyState('M'))
//	{
//		transfrom.tX += changeSpeed * 0.1f;
//	}
//}
//
//Matrix4 GetTransformMatrix(const Transform3D& theTransform)
//{
//	Matrix4 ret{};
//	ret.Identity();
//	Matrix4 newTransform_Translate;
//	newTransform_Translate.Translation(theTransform.tX, theTransform.tY, theTransform.tZ);
//
//	Matrix4 newTransform_Scale;
//	Vector4 scale(theTransform.scX, theTransform.scY, theTransform.scZ, 1.0f);
//	newTransform_Scale.Scaling3D(scale);
//
//	Matrix4 newTransform_rotation;
//	newTransform_rotation.Rotation(theTransform.rX, theTransform.rY, theTransform.rZ);
//
//	Matrix4 newTransform_Shear;
//	newTransform_Shear.Shear(theTransform.shX, theTransform.shY, theTransform.shZ);
//
//	ret = ret * newTransform_Scale * newTransform_Shear * newTransform_rotation * newTransform_Translate;
//
//	return ret;
//
//}
