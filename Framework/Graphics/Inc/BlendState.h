#ifndef INCLUDE_NFGE_GRAPHICS_BLENDSTATE_H
#define INCLUDE_NFGE_GRAPHICS_BLENDSTATE_H

namespace NFGE::Graphics
{
	class BlendState
	{
	public:
		enum class Mode
		{
			Opaque,
			AlphaBlend,
			AlphaPremultiplied,
			Additive
		};

		static void ClearState();

		BlendState() = default;
		~BlendState();

		BlendState(const BlendState&) = delete;
		BlendState& operator=(const BlendState&) = delete;

		void Initialize(Mode mode);
		void Terminate();

		void Set();

	private:
		ID3D11BlendState* mBlendState;
	};
}

#endif // !INCLUDE_NFGE_GRAPHICS_BLENDSTATE_H
