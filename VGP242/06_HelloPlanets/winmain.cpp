#include "Plante.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

void CameraControl(Camera& camera);

float radiusOfSun = 50.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup our application window
	int windowWidth = 1280;
	int windowHeight = 720;

	Window myWindow;

	myWindow.Initialize(hInstance, "Hello Plantes", windowWidth, windowHeight);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	
	std::vector<Planet*> myPlantes;

	MeshPX sunMesh = MeshBuilder::CreateSpherePX(20,20, radiusOfSun);
	//MeshPC sunMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet Sun{};
	Sun.mRadius = radiusOfSun;
	Sun.Load(sunMesh, L"../../Assets/Images/sun.jpg");
	//Sun.Load(sunMesh);
	PlanetMovment sunMovment{ };
	myPlantes.push_back(&Sun);

	MeshPX earthMesh = MeshBuilder::CreateSpherePX(20, 20, radiusOfSun * 0.1f);
	//MeshPC earthMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet Earth{};
	Earth.mRadius = radiusOfSun * 0.1f;
	Earth.Load(earthMesh, L"../../Assets/Images/earth.jpg");
	//Earth.Load(sunMesh);
	PlanetMovment earthMovment{};
	Earth.mInitTransfromInfo.rZ = 90.0f;
	myPlantes.push_back(&Earth);

	MeshPX moonMesh = MeshBuilder::CreateSpherePX(20, 20, radiusOfSun * 0.01f);
	//MeshPC moonMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet Moon{};
	Moon.mRadius = radiusOfSun * 0.01f;
	Moon.Load(moonMesh, L"../../Assets/Images/moon.jpg");
	//Moon.Load(sunMesh);
	PlanetMovment moonMovment{ };
	myPlantes.push_back(&Moon);

	MeshPX mercuryMesh = MeshBuilder::CreateSpherePX(20, 20, radiusOfSun * 0.1f);
	//MeshPC earthMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet Mercury{};
	Mercury.mRadius = radiusOfSun * 0.1f;
	Mercury.Load(mercuryMesh, L"../../Assets/Images/8k_mercury.jpg");
	//Earth.Load(sunMesh);
	PlanetMovment mercuryMovment{};
	Mercury.mInitTransfromInfo.rZ = 90.0f;
	myPlantes.push_back(&Mercury);

	MeshPX venusMesh = MeshBuilder::CreateSpherePX(20, 20, radiusOfSun * 0.125f);
	//MeshPC earthMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet Venus{};
	Venus.mRadius = radiusOfSun * 0.125f;
	Venus.Load(venusMesh, L"../../Assets/Images/8k_venus.jpg");
	//Earth.Load(sunMesh);
	PlanetMovment venusMovment{};
	Venus.mInitTransfromInfo.rZ = 90.0f;
	myPlantes.push_back(&Venus);

	MeshPX JupiterMesh = MeshBuilder::CreateSpherePX(20, 20, radiusOfSun * 0.4f);
	//MeshPC earthMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet Jupiter{};
	Jupiter.mRadius = radiusOfSun * 0.4f;
	Jupiter.Load(JupiterMesh, L"../../Assets/Images/8k_jupiter.jpg");
	//Earth.Load(sunMesh);
	PlanetMovment jupiterMovment{};
	Jupiter.mInitTransfromInfo.rZ = 90.0f;
	myPlantes.push_back(&Jupiter);

	MeshPX saturnMesh = MeshBuilder::CreateSpherePX(20, 20, radiusOfSun * 0.3f);
	//MeshPC earthMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet Saturn{};
	Saturn.mRadius = radiusOfSun * 0.3f;
	Saturn.Load(saturnMesh, L"../../Assets/Images/8k_saturn.jpg");
	//Earth.Load(sunMesh);
	PlanetMovment saturnMovment{};
	Saturn.mInitTransfromInfo.rZ = 90.0f;
	myPlantes.push_back(&Saturn);

	MeshPX saturnRingMesh = MeshBuilder::CreateTorusPX(20, 20, radiusOfSun * 0.325f, radiusOfSun * 0.6f);
	//MeshPC earthMesh = MeshBuilder::CreateSpherePC(20,20, radiusOfSun)();
	Planet SaturnRing{};
	SaturnRing.mRadius = radiusOfSun * 0.3f;
	SaturnRing.Load(saturnRingMesh, L"../../Assets/Images/8k_saturn_ring_alpha.png");
	//Earth.Load(sunMesh);
	PlanetMovment saturnRingMovment{};
	SaturnRing.mInitTransfromInfo.rY = 90.0f;
	SaturnRing.mInitTransfromInfo.scX = 0.01f;
	myPlantes.push_back(&SaturnRing);


	//Create Universe
	MeshPC universeMesh = MeshBuilder::CreateFlipCubePC(2000.0f);
	Planet universe{};
	universe.Load(universeMesh);

	Camera camera;
	camera.SetPosition({ 0.0f, 0.0f, -500.0f });
	camera.SetDirection({ 0.0f, 0.0f, 1.0f });
	camera.SetFarPlane(5000.0f);

	bool done = false;
	size_t currentControlPlante = 0;

	while (!done)
	{
		size_t totalMesh = myPlantes.size();
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}


		
		myPlantes[currentControlPlante]->Control();


		sunMovment.SroundTarget = { 0.0f,0.0f,0.0f };
		sunMovment.SelfRotate.x += 0.01f;
		Sun.Update(sunMovment);

		mercuryMovment.SroundTarget = Sun.mPosition;
		mercuryMovment.SroundDistance.x = 2.5f * radiusOfSun;
		mercuryMovment.SelfRotate.x += 0.5f;			//earthMovment.SelfRotate.y += 2.0f;
		mercuryMovment.SroundRotate.x += 0.0f;		mercuryMovment.SroundRotate.y += 0.0f; mercuryMovment.SroundRotate.z += 0.5f;
		Mercury.Update(mercuryMovment);

		venusMovment.SroundTarget = Sun.mPosition;
		venusMovment.SroundDistance.x = 3.0f * radiusOfSun;
		venusMovment.SelfRotate.x += 0.5f;			//earthMovment.SelfRotate.y += 2.0f;
		venusMovment.SroundRotate.x += 0.0f;		venusMovment.SroundRotate.y += 0.0f; venusMovment.SroundRotate.z += 0.9f;
		Venus.Update(venusMovment);

		earthMovment.SroundTarget = Sun.mPosition;
		earthMovment.SroundDistance.x = 4.0f * radiusOfSun;
		earthMovment.SelfRotate.x += 0.5f;			//earthMovment.SelfRotate.y += 2.0f;
		earthMovment.SroundRotate.x += 0.0f;		earthMovment.SroundRotate.y += 0.0f; earthMovment.SroundRotate.z += 0.8f;
		Earth.Update(earthMovment);

		moonMovment.SroundDistance = 2.0f * radiusOfSun * 0.1f;
		moonMovment.SroundTarget = Earth.mPosition;
		moonMovment.SelfRotate.x += 0.01f;			//earthMovment.SelfRotate.y += 2.0f;
		moonMovment.SroundRotate.x += 0.0f;		moonMovment.SroundRotate.y += 0.0f; moonMovment.SroundRotate.z += 2.0f;
		Moon.Update(moonMovment);

		jupiterMovment.SroundTarget = Sun.mPosition;
		jupiterMovment.SroundDistance.x = 4.5f * radiusOfSun;
		jupiterMovment.SelfRotate.x += 0.5f;			//earthMovment.SelfRotate.y += 2.0f;
		jupiterMovment.SroundRotate.x += 0.0f;		jupiterMovment.SroundRotate.y += 0.0f; jupiterMovment.SroundRotate.z += 0.45f;
		Jupiter.Update(jupiterMovment);

		saturnMovment.SroundTarget = Sun.mPosition;
		saturnMovment.SroundDistance.x = 6.0f * radiusOfSun;
		saturnMovment.SelfRotate.x += 0.5f;			//earthMovment.SelfRotate.y += 2.0f;
		saturnMovment.SroundRotate.x += 0.0f;		saturnMovment.SroundRotate.y += 0.0f; saturnMovment.SroundRotate.z += 0.4f;
		Saturn.Update(saturnMovment);

		saturnRingMovment.SroundTarget = Sun.mPosition;
		saturnRingMovment.SroundDistance.x = 6.0f * radiusOfSun;
		saturnRingMovment.SelfRotate.x += 1.0f;			//earthMovment.SelfRotate.y += 2.0f;
		saturnRingMovment.SroundRotate.x += 0.0f;		saturnRingMovment.SroundRotate.y += 0.0f; saturnRingMovment.SroundRotate.z += 0.4f;
		SaturnRing.Update(saturnRingMovment);
		
		//Camera control
		CameraControl(camera);
		if (GetAsyncKeyState('G')) // Master View
		{
			camera.SetPosition({ 500.0f,500.0f,500.0f });
			camera.SetDirection(Sun.mPosition - camera.GetPosition());
		}

		//Plant control
		for (int i = 0; i < 10; i++)
		{
			if (GetAsyncKeyState('0' + i))
			{
				currentControlPlante = i % (myPlantes.size());
				auto& plant = (myPlantes[currentControlPlante]);
				camera.SetPosition(plant->mPosition + Vector3{ 0.0f* plant->mRadius,0.0f* plant->mRadius ,10.0f* plant->mRadius });
				camera.SetDirection(plant->mPosition - camera.GetPosition());
				break;
			}
		}

		GraphicsSystem::Get()->BeginRender();

		universe.Render(camera, windowWidth, windowHeight);
		for (auto& plante : myPlantes)
		{
			plante->Render(camera, windowWidth, windowHeight);
		}

		GraphicsSystem::Get()->EndRender();
	}

	universe.Unload();
	for (auto& plante : myPlantes)
	{
		plante->Unload();
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


