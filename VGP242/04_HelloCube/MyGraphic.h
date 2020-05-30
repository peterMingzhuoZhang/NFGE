#ifndef HELLOCUBE_MYGRAPHIC_H
#define HELLOCUBE_MYGRAPHIC_H

#include <Core\Inc\Core.h>
#include <Graphics\Inc\Graphics.h>
#include <NFGEMath/Inc/NFGEMath.h>

#include "Camera.h"
#include "MatrixManager.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Math;

struct PTContext
{
	VertexPC* mVertices;
	uint32_t mVerticesCount;
	uint32_t* mIndexBuffer;
	uint32_t mIndexCount;
};

class MyGraphic
{
public:
	MyGraphic();
	~MyGraphic();

	void Initialize(Window* theWindow, int theWindowWidth, int theWindowHeight);
	void Terminate();

	void Update();
	void Render();

	void Geometry3DTransform(VertexPC* theVertices, VertexPC* newVertices, uint32_t size);

private:
	void LoadContext();
private:
	int mWindowWidth;
	int mWindowHeight;
	Window* mWindow = nullptr;
	int currentMesh = 0;
	std::vector<MeshBuffer> myMeshes;
	VertexShader vertexShader;
	PixelShader pixelShader;

	//Context
	std::vector<PTContext> myCubeContexts;
	float ratio;
};

#endif // !HELLOCUBE_MYGRAPHIC_H
