#include <Engine/NFGE/Inc/NFGE.h>
#include <random>

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup our application window
	Window myWindow;
	int windowWidth = 1280;
	int windowHeight = 720;

	myWindow.Initialize(hInstance, "Hello Shapes", windowWidth, windowHeight);

	// Initialize the graphics system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Define our vertex data
	enum MyShapes
	{
		Hreat = 0,
		TriForce,
		Unknow
	};
	std::vector<MeshBuffer> myMeshes;

	float randomR = static_cast<float>(std::rand() % 256) / 255.0f;
	float randomG = static_cast<float>(std::rand() % 256) / 255.0f;
	float randomB = static_cast<float>(std::rand() % 256) / 255.0f;

	float ratio = static_cast<float>(windowHeight) / static_cast<float>(windowWidth);
	// Heart
	const VertexPC HeartVertices[] =
	{
		{ { -0.4f * ratio, 0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { -0.2f * ratio, 0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { -0.6f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},

		{ { -0.2f * ratio, 0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { -0.0f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { -0.6f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},

		{ { 0.2f * ratio, 0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.6f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.0f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},

		{ { 0.2f * ratio, 0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.4f * ratio, 0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.6f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},

		{ { -0.6f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.0f  * ratio, -0.7f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { -0.6f * ratio, -0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},

		{ { -0.6f * ratio, -0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.0f  * ratio, -0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.0f  * ratio, -0.7f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},

		{ { 0.0f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.6f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.0f * ratio, -0.7f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},

		{ { 0.6f * ratio, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
		{ { 0.6f * ratio, -0.2f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
		{ { 0.0f * ratio, -0.7f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
		
		// Extra

	};
	MeshBuffer heartMesh{};
	heartMesh.Initialize(HeartVertices, static_cast<uint32_t>(std::size(HeartVertices)), sizeof(VertexPC));
	myMeshes.push_back(heartMesh);

	// TriForce
	const VertexPC TriForceVertices[] =
	{
		{ {  0.0f,  0.2f,  0.0f }, {0.95f, 0.81f, 0.25f, 1.0f}},
		{ {  0.1f, 0.0f,  0.0f }, {0.95f, 0.81f, 0.25f,  1.0f }},
		{ { -0.1f, 0.0f,  0.0f }, {0.95f, 0.81f, 0.25f,  1.0f }},

		{ {  -0.1f,  0.0f,  0.0f },{ 0.95f, 0.81f, 0.25f, 1.0f}},
		{ {  0.0f, -0.2f,  0.0f }, {0.95f, 0.81f, 0.25f,  1.0f }},
		{ { -0.2f, -0.2f,  0.0f }, {0.95f, 0.81f, 0.25f,  1.0f }},

		{ {  0.1f,  0.0f,  0.0f }, {0.95f, 0.81f, 0.25f, 1.0f }},
		{ {  0.2f, -0.2f,  0.0f }, {0.95f, 0.81f, 0.25f, 1.0f }},
		{ {  0.0f, -0.2f,  0.0f }, {0.95f, 0.81f, 0.25f, 1.0f }}
	};
	MeshBuffer TriForceMesh{};
	TriForceMesh.Initialize(TriForceVertices, static_cast<uint32_t>(std::size(TriForceVertices)), sizeof(VertexPC));
	myMeshes.push_back(TriForceMesh);
	

	// Unknow
	const VertexPC unknowVertices[] =
	{
		{ {  -0.52f	* ratio,  0.3f,	0.0f },  {0.2f, 0.2f, 0.2f, 1.0f}},
		{ {  0.0f	* ratio,  0.0f,	0.0f },  {0.2f, 0.2f, 0.2f, 1.0f}},
		{ {  0.0f	* ratio, -0.3f,	0.0f },  {0.2f, 0.2f, 0.2f, 1.0f}},

		{ {  0.0f	* ratio,  0.0f,  0.0f },  {0.4f, 0.4f, 0.4f, 1.0f}},
		{ {  0.26f	* ratio,  0.15f,  0.0f },  {0.4f, 0.4f, 0.4f, 1.0f}},
		{ {  0.0f	* ratio, -0.6f,  0.0f },  {0.4f, 0.4f, 0.4f, 1.0f}},

		{ {  -0.26f	* ratio,  0.15f, 0.0f },  {0.9f, 0.9f, 0.9f, 1.0f}},
		{ {  0.52f	* ratio,  0.3f,   0.0f },  {0.9f, 0.9f, 0.9f, 1.0f}},
		{ {  0.0f	* ratio,  0.0f,   0.0f },  {0.9f, 0.9f, 0.9f, 1.0f}},

		{ {  -0.52f	* ratio, -0.3f,  0.0f },  {0.2f, 0.2f, 0.2f, 1.0f}},
		{ {  0.0f	* ratio, -0.3f,  0.0f },  {0.2f, 0.2f, 0.2f, 1.0f}},
		{ {  0.0f	* ratio, -0.6f,  0.0f },  {0.2f, 0.2f, 0.2f, 1.0f}},

		{ {  0.26f	* ratio,  0.15f,  0.0f },  {0.4f, 0.4f, 0.4f, 1.0f}},
		{ {  0.52f	* ratio,  0.3f,  0.0f },  {0.4f, 0.4f, 0.4f, 1.0f}},
		{ {  0.52f	* ratio, -0.3f,  0.0f },  {0.4f, 0.4f, 0.4f, 1.0f}},

		{ {  -0.52f	* ratio,  0.3f,  0.0f },   {0.9f, 0.9f, 0.9f, 1.0f}},
		{ {  0.0f	* ratio,  0.52f,  0.0f },  {0.9f - 0.2f, 0.9f - 0.2f, 0.9f - 0.2f, 1.0f}}, // ------------ darker
		{ {  -0.26f	* ratio,  0.15f,  0.0f }, {0.9f, 0.9f, 0.9f, 1.0f}},

		{ {  0.13f	* ratio, -0.225f, 0.0f },  {0.95f - 0.2f , 0.95f - 0.2f , 0.95f - 0.2f, 1.0f}}, // ------------- darker
		{ {  0.26f	* ratio,  0.15f,   0.0f },  {0.95f, 0.95f, 0.95f, 1.0f}},
		{ {  0.52f	* ratio, -0.3f,   0.0f },  {0.95f, 0.95f, 0.95f, 1.0f}},

		{ {  -0.26f	* ratio,  0.15f, 0.0f },  {0.95f, 0.95f, 0.95f, 1.0f}},
		{ {  0.0f	* ratio,  0.52f,   0.0f }, { 0.95f, 0.95f, 0.95f, 1.0f}},
		{ {  0.13f	* ratio,  0.225f,   0.0f }, { 0.95f - 0.2f , 0.95f - 0.2f, 0.95f - 0.2f, 1.0f}}, // ----------- darker

		{ {  -0.52f	* ratio, -0.3f, 0.0f },  {0.95f, 0.95f, 0.95f, 1.0f}},
		{ {  -0.26f	* ratio,  0.0f,   0.0f }, { 0.95f - 0.2f , 0.95f - 0.2f, 0.95f - 0.2f, 1.0f}}, // ------------- darker
		{ {  0.0f	* ratio, -0.3f,   0.0f },  {0.95f, 0.95f, 0.95f, 1.0f}},
	};
	MeshBuffer unknowMesh{};
	unknowMesh.Initialize(unknowVertices, static_cast<uint32_t>(std::size(unknowVertices)), sizeof(VertexPC));
	myMeshes.push_back(unknowMesh);

	VertexShader vertexShader{};
	vertexShader.Initialize(L"../../Assets/Shaders/DoSomething.fx", VertexPC::Format);

	PixelShader pixelShader{};
	pixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");




	Camera camera;
	camera.SetPosition({ 0.0f, 0.0f, -10.0f });
	camera.SetDirection({ 0.0f, 0.0f, 1.0f });

	bool done = false;
	MyShapes currentShape = MyShapes::Hreat;

	while (!done)
	{
		
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		else if (GetAsyncKeyState(VK_NUMPAD1))
		{
			currentShape = MyShapes::Hreat;
		}
		else if (GetAsyncKeyState(VK_NUMPAD2))
		{
			currentShape = MyShapes::TriForce;
		}
		else if (GetAsyncKeyState(VK_NUMPAD3))
		{
			currentShape = MyShapes::Unknow;
		}

		// Run your game logic ...
		GraphicsSystem::Get()->BeginRender();

	

		vertexShader.Bind();
		pixelShader.Bind();

		

		myMeshes[currentShape].Render();

		GraphicsSystem::Get()->EndRender();
	}


	vertexShader.Terminate();
	pixelShader.Terminate();
	for (auto& mesh : myMeshes)
	{
		mesh.Terminate();
	}

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}