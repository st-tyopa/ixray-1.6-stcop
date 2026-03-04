#pragma once
#include "../UIXDefs.h"

enum class EUIXDrawType : u8
{
    none,
    box,
    border,
    image
};

enum class EUIXTiling : u8
{
    none,
    vertical,
    horizontal,
    both
};

//#define UI_X_BRUSH_VALID_SIZE (1<<0)
//#define UI_X_BRUSH_VALID_RECT (1<<1)

#define UI_X_DEFAULT_TEXTURE_NAME "$alphadxt1"
//#define UI_X_DEFAULT_SHADER_NAME "hud\\default"
#define UI_X_DEFAULT_SHADER_NAME "hud\\fog_of_war"

class UI_API CUIXBrush
{
    //flags8 _flags;
    bool bHasSize;
	ui_x_shader m_shader;

    /** visual */
    u32 m_tint;
    xr_vector2f m_size;
    EUIXTiling m_tile;
    EUIXDrawType m_drawType;
public:
    CUIXBrush();
    CUIXBrush(shared_str const& textureName);
    CUIXBrush(shared_str const& textureName, shared_str const& shaderName);
    ~CUIXBrush() {}
    
    void CreateShader   (shared_str const& textureName = UI_X_DEFAULT_TEXTURE_NAME, shared_str const& shaderName = UI_X_DEFAULT_SHADER_NAME);
    void Render         (const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform);

    void SetTint        (const u8 alpha, const u8 red, const u8 green, const u8 blue) { m_tint = (alpha << 24) + (red << 16) + (green << 8) + blue; }
    void SetTint        (const u32 tint) { m_tint = tint; }
    u32 GetTint         () const { return m_tint; }
    void SetSize        (const xr_vector2f& size) { m_size.set(size); bHasSize = true; }
    void SetTile        (EUIXTiling tiling) { m_tile = tiling; }
    void SetDrawType    (EUIXDrawType drawType) { m_drawType = drawType; }

#ifdef DEBUG_DRAW
    shared_str m_DebugTextureName;
    shared_str m_DebugShaderName;
    void RenderUIDebugProperties();
#endif
    
private:
    void RenderInternal (const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform);
};
