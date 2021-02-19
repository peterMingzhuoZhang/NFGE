#include <NFGE/Inc/NFGE.h>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

const wchar_t* shaderFileName = L"../Assets/Shaders/SimpleDraw.fx";

void CameraControl(Camera& camera);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup our application window
	int windowWidth = 1280;
	int windowHeight = 720;

	Window myWindow;

	myWindow.Initialize(hInstance, "Hello Shapes", windowWidth, windowHeight);

	// Initialize the input system
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());

	// Initialize the graphic system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	BlendStateManager::StaticInitialize();
	SimpleDraw::StaticInitialize(L"../../Assets/Shaders");

	DebugUI::StaticInitialize(myWindow.GetWindowHandle(),true);

	Camera camera;
	camera.SetPosition({ 0.0f, 0.0f, -10.0f });
	camera.SetDirection({ 0.0f, 0.0f, 1.0f });


	bool done = false;
	size_t currentMesh = 0;

	while (!done)
	{
		done = myWindow.ProcessMessage();

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			done = true;
		}

		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
		lastTime = currentTime;

		const float moveSpeed = 10.0f;
		const float turnSpeed = 5.0f;

		if (inputSystem->IsKeyDown(KeyCode::W))
			camera.Walk(moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::S))
			camera.Walk(-moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::D))
			camera.Strafe(moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::A))
			camera.Strafe(-moveSpeed * deltaTime);
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		{
			camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
			camera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
		}



		//CameraControl(camera);



		for (float i = -50.0f; i <= 50.0f; i += 1.0f)
		{
			SimpleDraw::AddLine({ i,0.0f,-50.0f }, { i,0.0f,50.0f }, Colors::DarkGray);
			SimpleDraw::AddLine({ -50.0f,0.0f,i }, { 50.0f,0.0f,i }, Colors::DarkGray);
		}

		SimpleDraw::AddLine({ 0.0f, 0.0f,0.0f }, Vector3::YAxis, Colors::Black);

		SimpleDraw::AddTriangle({ 2.0f, 2.0f,0.0f }, { 2.0f, 0.0f,0.0f }, { 1.5f, 0.0f,1.0f }, Colors::Black);

		SimpleDraw::AddAABB({ -0.0f, 0.0, -3.0 }, 1.0f, Colors::OrangeRed);

		SimpleDraw::AddSphere({ 3.0f, 0.0, -3.0 }, 1.0f, Colors::OrangeRed, 8, 8);

		SimpleDraw::AddScreenCircle({ 5.0f,5.0f }, 5.0f, Colors::Red);

		GraphicsSystem::Get()->BeginRender();
		SimpleDraw::Render(camera);
		DebugUI::BeginRender();
		ImGui::ShowDemoWindow();
		DebugUI::EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	DebugUI::StaticTerminate();
	InputSystem::StaticTerminate();
	BlendStateManager::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	SimpleDraw::StaticTerminate();
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


