#ifndef FontRender_included
#define FontRender_included
#pragma once

class CGameFont;

struct FFontGlyphVertex2d
{
	Fvector4 p; u32 color; Fvector2    uv;
};
struct FFontGlyphPoly2d
{
	FFontGlyphVertex2d buff[4];
};

class IFontRender
{
public:
	virtual ~IFontRender() {;}

	virtual void Initialize(LPCSTR cShader, LPCSTR cTexture) = 0;
	virtual void OnRender(CGameFont &owner) = 0;

	virtual void CreateFontAtlas(u32 width, u32 height, const char* name, void* bitmap) = 0;

	virtual void ApplyShader() = 0;
	virtual void StartText(u32 textLength) = 0;
	virtual void PushGlyph(const Frect& pt, const Frect& uv, u32 color) = 0;
	virtual void FlushText() = 0;
};

#endif	//	FontRender_included