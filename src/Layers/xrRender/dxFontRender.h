#pragma once

#include "../../Include/xrRender/FontRender.h"

class dxFontRender : public IFontRender
{
public:
	dxFontRender();
	virtual ~dxFontRender();

	virtual void Initialize(const char* cShader, const char* cTexture);
	virtual void OnRender(CGameFont& owner);

	virtual void CreateFontAtlas(u32 width, u32 height, const char* name, void* bitmap) override;

	void ApplyShader() override;
	void StartText(u32 textLength) override;
	void PushGlyph(const Frect& pt, const Frect& uv, u32 color) override;
	void FlushText() override;

private:
	ref_shader				pShader;
	ref_geom				pGeom;
	ref_texture				pTexture;

	u32                        m_vOffset;
	u32                        m_vertexCount;    
	FFontGlyphPoly2d*        m_pVertexes;
};