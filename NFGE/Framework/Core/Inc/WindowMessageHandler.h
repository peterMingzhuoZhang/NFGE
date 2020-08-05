#ifndef INCLUDE_NFGE_CORE_WINDOWMESSAGEHANDLER_H
#define INCLUDE_NFGE_CORE_WINDOWMESSAGEHANDLER_H

namespace NFGE::Core {

	class WindowMessageHandler
	{
	public:
		using Callback = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

		void Hook(HWND window, Callback cb);
		void Unhook();

		LRESULT ForwardMessage(HWND window, UINT messgae, WPARAM wparam, LPARAM lParam);

	private:
		HWND mWindow{ nullptr };
		Callback mPreviousCallback{ nullptr };
	};

} // namespace NFGE::Core

#endif // !INCLUDE_NFGE_CORE_WINDOWMESSAGEHANDLER_H
