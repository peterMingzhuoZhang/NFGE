 #ifndef NFGE_GRAPHIC_MESHIO_H
#define NFGE_GRAPHIC_MESHIO_H

namespace NFGE::Graphics
{
	template <typename T>
	struct MeshBase;
	struct BoneVertex;
	using BoneMesh = MeshBase<BoneVertex>;

	namespace MeshIO
	{
		bool Save(const BoneMesh& mesh, FILE* file, NFGE::Core::IOOption option = NFGE::Core::IOOption::Binary);
		bool Load(BoneMesh& mesh, FILE* file, NFGE::Core::IOOption option = NFGE::Core::IOOption::Binary);
	}
}

#endif // !NFGE_GRAPHIC_MESHIO_H
