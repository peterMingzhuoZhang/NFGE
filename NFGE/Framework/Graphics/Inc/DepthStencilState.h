#ifndef NFGE_GRAPHIC_DEPTHSTENCILSTATE_H
#define NFGE_GRAPHIC_DEPTHSTENCILSTATE_H

namespace NFGE::Graphics
{


class DepthStencilState
{
public:
	enum class StateMode { DepthDefault, DepthNone, DepthRead};

	DepthStencilState() = default;
	~DepthStencilState();

	DepthStencilState(const DepthStencilState&) = delete;
	DepthStencilState& operator=(const DepthStencilState&) = delete;

	void Initialize(StateMode stateMode);
	void Terminate();

	void Set();
	void Clear();

private:
	ID3D11DepthStencilState * mDepthStencilState{ nullptr };
};

} // namespace NFEG::Graphics

#endif // !NFGE_GRAPHIC_DEPTHSTENCILSTATE_H
