//====================================================================================================
// Filename:	Font.cpp
// Created by:	Peter Chan (Peter Chan is Awesome)
//====================================================================================================

#include "Precompiled.h"
#include "Font.h"
#include "D3DUtil.h"

#include <FW1FontWrapper_1_1/FW1FontWrapper.h>

#pragma comment(lib, "FW1FontWrapper.lib")

using namespace NFGE;

//----------------------------------------------------------------------------------------------------

Font::Font()
	: mFW1Factory(nullptr)
	, mFontWrapper(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

Font::~Font()
{
}

//----------------------------------------------------------------------------------------------------

void Font::Initialize()
{
	ID3D11Device* device = NFGE::Graphics::GetDevice();
	FW1CreateFactory(FW1_VERSION, &mFW1Factory);
	mFW1Factory->CreateFontWrapper(device, L"Consolas", &mFontWrapper);
}

//----------------------------------------------------------------------------------------------------

void Font::Terminate()
{
	SafeRelease(mFontWrapper);
	SafeRelease(mFW1Factory);
}

//----------------------------------------------------------------------------------------------------

void Font::Draw(const wchar_t* str, float x, float y, float size, uint32_t color)
{
	ID3D11DeviceContext* context = NFGE::Graphics::GetContext();
	mFontWrapper->DrawString(context, str, x, y, size, color, FW1_RESTORESTATE);
}