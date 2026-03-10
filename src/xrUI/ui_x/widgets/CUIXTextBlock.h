#pragma once
#include "ui_x/common/CUIXWidget.h"
#include "../../xrScripts/script_export_space.h"

struct SUIXTextElement
{
    xr_vector2f Position;
    u32 StartIndex;
    u32 Length;
    u32 Color;
    u32 lineIndex; // has no idea how to do horizontal alignment in another way
};



class CUIXTextBlock : public CUIXWidget
{
    using inherited = CUIXWidget;
    
    u32 m_color;
    xr_string m_pText;
    CGameFont* m_pFont;
    bool m_bMultiline = false;
    EUIXAlignVertical m_eAlignV;
    EUIXAlignHorizontal m_eAlignH;

    bool m_bIsUtf8 = false;
    u32 m_glyphCount = 0;
    xr_vector<float> m_textLinesWidth; // has no idea how to do horizontal alignment in another way
    xr_list<SUIXTextElement> m_Elements;
public:
    // --- Constructors -----------------------------------------------
    CUIXTextBlock();
    ~CUIXTextBlock() override;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXTextBlock* ui_x_cast_text() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Draw() override;
    void Rebuild() override;  
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    void SetColor       (const u8 alpha, const u8 red, const u8 green, const u8 blue) { m_color = (alpha << 24) + (red << 16) + (green << 8) + blue; }
    void SetColor       (const u32& color) { m_color = color; }
    u32 GetColor() const { return m_color; }
    
    void SetFont        (CGameFont* font);
    CGameFont* GetFont  () const { return m_pFont; }
    
    void SetText        (shared_str const& text);
    const xr_string& GetText() const { return m_pText; }
    
    void SetAlignVert   (const EUIXAlignVertical& align) { m_eAlignV = align; }
    EUIXAlignVertical GetAlignVert() const { return m_eAlignV; }
    
    void SetAlignHorz   (const EUIXAlignHorizontal& align) { m_eAlignH = align; }
    EUIXAlignHorizontal GetAlignHorz() const { return m_eAlignH; }
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    xr_string m_DebugText;
    xr_string m_DebugFontName;
    void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
private:
    // --- Utils ------------------------------------------------------
    static xr_string ReadAttr(const xr_string& tagBody, const xr_string& atrrName, const xr_string& defaultValue);
    void PushElement(u32 startIndex, u32 length, xr_vector2f position, u32 color, u32 currentLineIndex);
    void NextLine(const u32& symbolIndex, u32& elementIndex, float& currentX, float& currentW, u32& currentLineIndex);
    // ----------------------------------------------------------------
};
