#pragma once
#include "../UIXDefs.h"
#include "../../xrScripts/script_export_space.h"

enum class EUIXDrawType : u8
{
    image,
    border,
    box,
    none
};

enum class EUIXTiling : u8
{
    none,
    vertical,
    horizontal,
    both
};

#define UI_X_DEFAULT_TEXTURE_NAME "$alphadxt1"
//#define UI_X_DEFAULT_SHADER_NAME "hud\\default"
#define UI_X_DEFAULT_SHADER_NAME "hud\\fog_of_war"



class UI_API CUIXBrush
{
    // for draw type image or central part in draw type box
	ui_x_shader m_shader;   
     // frame sides in draw type box or draw type frame
    ui_x_shader m_frameLeftTop;
    ui_x_shader m_frameLeft;
    ui_x_shader m_frameLeftBottom;
    ui_x_shader m_frameRightTop;
    ui_x_shader m_frameRight;
    ui_x_shader m_frameRightBottom;
    ui_x_shader m_frameTop;
    ui_x_shader m_frameBottom;

    /** visual */
    xr_rect_f m_uv;
    xr_vector2f m_size;
    u32 m_tint;
    EUIXTiling m_tile = EUIXTiling::none;
    EUIXDrawType m_drawType = EUIXDrawType::image;
public:
    // --- Constructors -----------------------------------------------
    CUIXBrush(EUIXDrawType drawType = EUIXDrawType::none);
    CUIXBrush(shared_str const& textureName, EUIXDrawType drawType = EUIXDrawType::image);
    CUIXBrush(shared_str const& textureName, shared_str const& shaderName, EUIXDrawType drawType = EUIXDrawType::image);
    virtual ~CUIXBrush() = default;
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void CreateShader   (shared_str const& textureName = UI_X_DEFAULT_TEXTURE_NAME, shared_str const& shaderName = UI_X_DEFAULT_SHADER_NAME);
    void Render         (const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform);
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    void SetTint        (const u8 alpha, const u8 red, const u8 green, const u8 blue) { m_tint = (alpha << 24) + (red << 16) + (green << 8) + blue; }
    void SetTint        (const u32 tint) { m_tint = tint; }
    u32 GetTint         () const { return m_tint; }
    
    void SetSize        (const xr_vector2f& size) { m_size.set(size); }
    xr_vector2f GetSize() const { return m_size; }
    
    void SetTile        (EUIXTiling tiling) { m_tile = tiling; }
    EUIXTiling GetTile() const { return m_tile; }
    
    void SetDrawType    (EUIXDrawType drawType) { m_drawType = drawType; }
    EUIXDrawType GetDrawType() const { return m_drawType; }
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    shared_str m_DebugTextureName;
    shared_str m_DebugShaderName;
    void RenderUIDebugProperties();
#endif
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
private:
    // --- Control ----------------------------------------------------
    void RenderInternal (EUIXTiling tile, const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform) const;
    void CreateFrameShader(shared_str const& textureName, shared_str const& shaderName);
    // ----------------------------------------------------------------
};
