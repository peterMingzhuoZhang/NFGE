#ifndef INCLUDED_NFGE_GRAPHICS_SPHEREMESH_H
#define INCLUDED_NFGE_GRAPHICS_SPHEREMESH_H

#include "GeometryMesh.h"

namespace NFGE::Graphics::Geometry
{
	class PlaneMesh : public NFGE::Graphics::GeometryMesh
	{
	public:
		void DefaultInitialize(NFGE::Graphics::DirectionalLight* light);
		void Load(int row, int col, float unitLength, float widthScale, float lengthScale, DirectionalLight* light, const char* diffuse, const char* normal = nullptr, const char* speclur = nullptr, const char* displacement = nullptr);
	};

	class SphereMesh : public NFGE::Graphics::GeometryMesh
	{
	public:
		void DefaultInitialize(NFGE::Graphics::DirectionalLight* light);
		void Load(int HeightFactor, int roundFactor, float radius, DirectionalLight* light);
	};

	class ConeMesh : public NFGE::Graphics::GeometryMesh
	{
	public:
		void DefaultInitialize(NFGE::Graphics::DirectionalLight* light);
		void Load(int row, int col, float radius, float height, DirectionalLight* light);
	};
}



#endif // !INCLUDED_HELLOSKELETON_PTSPHERE_H

