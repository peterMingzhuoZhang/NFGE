//====================================================================================================
// Filename:	Font.h
// Created by:	Peter Chan (Peter Chan is Awesome)
//====================================================================================================

#ifndef INCLUDED_XENGINE_FONT_H
#define INCLUDED_XENGINE_FONT_H

interface IFW1Factory;
interface IFW1FontWrapper;

namespace NFGE {

class Font
{
public:
	Font();
	~Font();

	void Initialize();
	void Terminate();

	void Draw(const wchar_t* str, float x, float y, float size, uint32_t color);

private:
	IFW1Factory* mFW1Factory;
	IFW1FontWrapper* mFontWrapper;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_FONT_H