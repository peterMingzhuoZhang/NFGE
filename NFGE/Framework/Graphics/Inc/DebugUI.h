//====================================================================================================
// Filename:	DebugUI.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_NFGE_GRAPHICS_DEBUGUI_H
#define INCLUDED_NFGE_GRAPHICS_DEBUGUI_H

namespace NFGE::Graphics::DebugUI {

	enum class Theme
	{
		Classic,
		Dark,
		Light,
	};

void StaticInitialize(HWND window, bool docking = false, bool mutiViewport = false);
void StaticTerminate();
void BeginRender();
void EndRender();

void SetTheme(Theme theme);

} // namespace NFGE::Graphics::DebugUI

#endif // #ifndef INCLUDED_NFGE_GRAPHICS_DEBUGUI_H