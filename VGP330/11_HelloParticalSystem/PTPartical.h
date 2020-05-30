#ifndef HELLOPARTICALSYSTEM_PTPARTICLE_H
#define HELLOPARTICALSYSTEM_PTPARTICLE_H

#include <NFGE/Inc/NFGE.h>

using namespace NFGE;

//----------------------------------------//|
struct Particle							  //|
{										  //|
	Math::Vector3 position;				  //|
	Math::Vector3 velocity;				  //|--- Basic structure for a single particle.
	Math::Vector4 color;				  //|	 Add more control variable here if you want fancy Particle system
	float lifeTime{ 0.0f };				  //|
};										  //|
//----------------------------------------//|

// This is the Object that shoot particles in this Demo
class PTParticleEmitter
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();

	void SpawnPartice();

public:
	// Control Attribute
	int mCurrentParticleCount = 0;
	float mMaxLifeTime = 2.0f;
	int mParticlesCount = 1000;
	Math::Vector4 startColor{ 0.8f, 0.8f, 0.0f, 1.0f };
	Math::Vector4 endColor{ 1.0f, 0.0f, 0.0f, 1.0f };

	std::string mCurrentBlendState;

private:
	//----------------------------------------------//|
	// Implementation variables						//|
	std::vector<Particle> mParticles;				//|--- Our particle pool
	int mNextParticle;								//|
	//----------------------------------------------//|

	//-------------------------------------------------------------------------//|
	struct ConstantData														   //|
	{																		   //|
		NFGE::Math::Matrix4 wvp;											   //|
		NFGE::Math::Vector4 viewPosition;									   //|
		NFGE::Math::Vector3 viewUp;											   //|
		float paddding;														   //|
	};																		   //|--- Graphic variables
																			   //|
	NFGE::Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;		   //|
	NFGE::Graphics::VertexShader mVertexShader;								   //|
	NFGE::Graphics::GeometryShader mGeometryShader;							   //|-------------------- [NEW STUFF ADDED]: Add <GeometryShader.h> & <GeometryShader.cpp> struct in to Graphics lib
	NFGE::Graphics::PixelShader mPixelShader;								   //|                                        just copy <PixelShader> which you already have. But also add a UnBind() 
	NFGE::Graphics::TextureId mTextureId;									   //|                                        which you can get reference from <RenderTarget::UnBind()>
	//-------------------------------------------------------------------------//|

	NFGE::Graphics::MeshBuffer mMeshBuffer; // The Mesh of a particle emmiter is bascily a bounch of random vertices
											// we create new vector of vertices each frame base on current active particles
											// use those vertices to update the Mesh variable.
};

#endif // !HELLOPARTICALSYSTEM_PTPARTICLE_H
