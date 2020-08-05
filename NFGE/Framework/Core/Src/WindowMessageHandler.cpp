#include "Precompiled.h"
#include "WindowMessageHandler.h"

#include "Debug.h"

using namespace NFGE::Core;

void NFGE::Core::WindowMessageHandler::Hook(HWND window, Callback cb)
{
	mWindow = window;
	mPreviousCallback = (Callback)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)cb);
}

void NFGE::Core::WindowMessageHandler::Unhook()
{
	SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviousCallback);
	mWindow = nullptr;
}

LRESULT NFGE::Core::WindowMessageHandler::ForwardMessage(HWND window, UINT messgae, WPARAM wParam, LPARAM lParam)
{
	ASSERT(mPreviousCallback, "[WindowMessageHandler] No callback is hooked");
	return CallWindowProcA((WNDPROC)mPreviousCallback, window, messgae, wParam, lParam);
}
