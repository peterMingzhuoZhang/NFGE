#ifndef INCLUDED_NFGE_GRAPHICS_MESHBUILDER_H
#define INCLUDED_NFGE_GRAPHICS_MESHBUILDER_H

#include "Mesh.h"

namespace NFGE {
	namespace Graphics {
		class MeshBuilder
		{
		public:
			static MeshPC CreateCubePC();
			static MeshPC CreateFlipCubePC(float size);
			static MeshPC CreatePlanePC(int row, int col, float unit);
			static MeshPC CreateCylinderPC(int HeightFactor, int roundFactor, float radius, float height);
			static MeshPC CreateConePC(int HeightFactor, int roundFactor, float radius, float height);
			static MeshPC CreateSpherePC(int HeightFactor, int roundFactor, float radius);
			static MeshPC CreateTorusPC(int LengthFactor, int roundFactor, float innerRadius, float outerRadius);

			static MeshPX CreateCubePX();
			static MeshPX CreatePlanePX();
			static MeshPX CreatePlanePX(int row, int col, float unit);
			static MeshPX CreateCylinderPX(int HeightFactor, int roundFactor, float radius, float height);
			static MeshPX CreateConePX(int HeightFactor, int roundFactor, float radius, float height);
			static MeshPX CreateSpherePX(int HeightFactor, int roundFactor, float radius);
			static MeshPX CreateTorusPX(int LengthFactor, int roundFactor, float innerRadius, float outerRadius);
			static MeshPX CreateNDCQuad();

			static MeshPN CreateSpherePN(int HeightFactor, int roundFactor, float radius);
			static MeshPN CreateConePN(int HeightFactor, int roundFactor, float radius, float height);
			
			static Mesh CreatePlane(int row, int col, float unit);
			static Mesh CreateCube();
			static Mesh CreateCone(int HeightFactor, int roundFactor, float radius, float height);
			static Mesh CreateCylinder(int HeightFactor, int roundFactor, float radius, float height);
			static Mesh CreateSphere(int HeightFactor, int roundFactor, float radius);
			static Mesh CreateTorus(int LengthFactor, int roundFactor, float innerRadius, float outerRadius);

			static BoneMesh CreateTentacle(int numOfSeg, int sliceFactor, int roundFactor, float radius, float height, float(*radiusShaper)(float));

			static Math::AABB ComputeBound(const Mesh& mesh);
			static void ComputeNormals(Mesh& mesh);
		};
	}
}

#endif // !INCLUDED_NFGE_GRAPHICS_MESHBUILDER_H
