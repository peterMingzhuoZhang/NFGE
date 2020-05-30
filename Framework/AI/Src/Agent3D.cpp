#include "Precompiled.h"
#include  "Agent3D.h"

#include "AIWorld3D.h"

using namespace NFGE::AI;
using namespace NFGE;

Agent3D::Agent3D(AIWorld3D& world)
	:mWorld(world)
{
	world.RegisterAgent3D(this);
}

Agent3D::~Agent3D()
{
	mWorld.UnregisterAgent3D(this);
}

NFGE::Math::Matrix4 NFGE::AI::Agent3D::LocalToWorld()
{
	NFGE::Math::Vector3 u = heading;
	NFGE::Math::Vector3 r = NFGE::Math::Cross({ 0.0f, 1.0f, 0.0f }, u);
	NFGE::Math::Vector3 l = NFGE::Math::Cross(r, u);
	NFGE::Math::Vector3 p = { position.x, position.y, position.z };
	return
	{
		r.x, r.y, r.z,	0.0f,
		u.x, u.y, u.z, 0.0f,
		l.x, l.y, l.z,  0.0f,
		p.x, p.y, p.z, 1.0f
	};
}

NFGE::Math::Matrix4 AI::Agent3D::LocalToWorldMatrix()
{
	NFGE::Math::Matrix4 matTrans = NFGE::Math::Translation(position.x, position.y, position.z);

	NFGE::Math::Matrix4 matRot;
	
	if (NFGE::Math::IsZero(heading - NFGE::Math::Vector3{ 0.0f, 1.0f, 0.0f }))
		matRot.Identity();
	else
		matRot = NFGE::Math::MatrixRotationQuaternion(NFGE::Math::QuaternionFromTo({0.0f,1.0f,0.0f}, heading));

	return matRot * matTrans;
}

void AI::Agent3D::UpdateNeighbor()
{
	neighborhood.clear();
	mWorld.GetNeighborhood(neighborhood , NFGE::Math::Sphere(position, neighbourRange));
	//neighborhood = std::move(newNeighbour);
}
