#include "stdafx.h"
#include "CUIXTextBlock.h"
#include <luabind/luabind.hpp>

#include "../xrEngine/string_table.h"

CUIXTextBlock::CUIXTextBlock() :
    inherited("text"),
    m_color(0xFFFFFFFF),
    m_pFont(nullptr)
{
    m_eAlignV = EUIXAlignVertical::Top;
    m_eAlignH = EUIXAlignHorizontal::Left;
}

CUIXTextBlock::~CUIXTextBlock()
{
    m_Elements.clear();
}

void CUIXTextBlock::Draw()
{
    if (m_glyphCount == 0 || !m_pFont)
    {
        return;
    }
    m_pFont->GetFontRender()->ApplyShader();
    m_pFont->GetFontRender()->StartText(m_glyphCount);
    xr_vector2f atlasSize;
    UIRender->GetActiveTextureResolution(atlasSize);

    xr_special_char* UniStr = nullptr;
    if (m_bIsUtf8)
    {
        UniStr = Platform::ANSI_TO_TCHAR(m_pText.c_str());
    }
    else
    {
        UniStr = Platform::ANSI_TO_TCHAR(Platform::ANSI_TO_UTF8(m_pText.c_str()).c_str());
    }

    float height = m_pFont->GetHeight() * float(m_textLinesWidth.size());
    float offsetY = 0.0f;
    if (m_eAlignV == EUIXAlignVertical::Buttom)
    {
        offsetY = GetRect().width() - height;   
    }
    else if (m_eAlignV == EUIXAlignVertical::Center)
    {
        offsetY = (GetRect().height() - height) * 0.5f;
    }
    
    for (SUIXTextElement element : m_Elements)
    {
        float offsetX = 0.0f;
        if (m_eAlignH == EUIXAlignHorizontal::Right)
        {
            offsetX = GetRect().width() - m_textLinesWidth[element.lineIndex];   
        }
        else if (m_eAlignH == EUIXAlignHorizontal::Center)
        {
            offsetX = (GetRect().width() - m_textLinesWidth[element.lineIndex]) * 0.5f;
        }
        
        float curX = float(iFloor(GetRect().lt.x + element.Position.x + offsetX));
        float curY = float(iFloor(GetRect().lt.y + element.Position.y + offsetY));
        float curY2 = curY + m_pFont->GetHeight();
        curX -= 0.5f; curY -= 0.5f; curY2 -= 0.5f;

        for (u32 i = 0; i < element.Length; i++)
        {
            const CGameFont::Glyph* glyph = nullptr;
            if (m_bIsUtf8)
            {
                glyph = m_pFont->GetGlyphInfo((u8)m_pText[element.StartIndex + i]);
            }
            else
            {
                glyph = m_pFont->GetGlyphInfo(UniStr[element.StartIndex + i]);
            }
            if (!glyph)
            {
                continue;
            }
            
            if (i != 0)
            {
                curX += float(glyph->Abc.abcA);
            }
        
            float x1 = curX;
            float y1 = curY + float(glyph->yOffset); 
            float x2 = x1 + float(glyph->Abc.abcB);
            float y2 = curY2 + float(glyph->yOffset);
        
            float u1 = float(glyph->TextureCoord.left) / atlasSize.x;
            float v1 = float(glyph->TextureCoord.top) / atlasSize.y;
            float u2 = float(glyph->TextureCoord.right) / atlasSize.x;
            float v2 = float(glyph->TextureCoord.bottom) / atlasSize.y;
        
            m_pFont->GetFontRender()->PushGlyph(Frect().set(x1, y1, x2, y2), Frect().set(u1, v1, u2, v2), element.Color);
        
            curX = x2 + float(glyph->Abc.abcC);
        }
    }
    m_pFont->GetFontRender()->FlushText();
}

void CUIXTextBlock::SetFont(CGameFont* font)
{
    m_pFont = font;
#ifdef DEBUG_DRAW
    m_DebugFontName = font->GetName();
#endif
}

void CUIXTextBlock::SetText(shared_str const& text)
{
    m_pText = xr_string(g_pStringTable->translate(text.c_str()).c_str());
#ifdef DEBUG_DRAW
    m_DebugText = xr_string(text.c_str());
#endif
}

void CUIXTextBlock::Rebuild()
{
    m_glyphCount = 0;
    m_Elements.clear();
    m_textLinesWidth.clear();
    
    if (!m_pFont)
    {
        m_pFont = g_FontManager->GetFont("font_default");
    }
    
    xr_special_char* UniStr = nullptr; 
    m_bIsUtf8 = IsUTF8(m_pText.c_str());
    if (m_bIsUtf8)
    {
        UniStr = Platform::ANSI_TO_TCHAR(m_pText.c_str());
    }
    else
    {
        UniStr = Platform::ANSI_TO_TCHAR(Platform::ANSI_TO_UTF8(m_pText.c_str()).c_str());
    }
    
    float curX = 0;
    float curW = 0;
    u32 currentLineIndex = 0;
    u32 startElementIndex = 0;
    u32 currentColor = m_color;
    u32 length = xr_strlen(m_pText.c_str());
    const float fontHeight = m_pFont->GetHeight();
    
    for (u32 i = 0; i < length; i++)
    {
        if (m_pText.c_str()[i] == '<')
        {
            size_t endTag = m_pText.find('>', i);
            if (endTag != xr_string::npos)
            {
                xr_string tagBody = m_pText.substr(i + 1, endTag - i - 1);
                
                PushElement(startElementIndex, i - startElementIndex, xr_vector2f().set(curX, float(currentLineIndex) * fontHeight), currentColor, currentLineIndex);
                i = (u32) endTag;
                NextLine(i, startElementIndex, curX, curW, currentLineIndex);
                
                if (tagBody[0] != '/') // start tag
                {
                    xr_string tagName = tagBody.substr(0, tagBody.find(' '));
                    if (tagName == "clr")
                    {
                        xr_string colorCode = ReadAttr(tagBody, "code", "0xFFFFFFFF");
                        currentColor = (u32)strtoul(colorCode.c_str(), nullptr, 16);
                    }
                }
                else // end tag
                {
                    if (tagBody == "/clr")
                    {
                        currentColor = m_color;  
                    }
                }
                
                continue;
            }
        }
        
        const CGameFont::Glyph* glyph = nullptr;
        if (m_bIsUtf8)
        {
            glyph = m_pFont->GetGlyphInfo((u8)m_pText[i]);
        }
        else
        {
            glyph = m_pFont->GetGlyphInfo(UniStr[i]);
        }
        if (glyph == nullptr)
        {
            continue;
        }

        if (startElementIndex != i)
        {
            curW += float(glyph->Abc.abcA);
        }
        curW += float(glyph->Abc.abcB + glyph->Abc.abcC);

        if (m_pText.c_str()[i] == ' ' && startElementIndex != i)
        {
            PushElement(startElementIndex, i - startElementIndex, xr_vector2f().set(curX, float(currentLineIndex) * fontHeight), currentColor, currentLineIndex);
            NextLine(i, startElementIndex, curX, curW, currentLineIndex);
        }
    }
    if (length - startElementIndex > 0)
    {
        PushElement(startElementIndex, length - startElementIndex, xr_vector2f().set(curX, float(currentLineIndex) * fontHeight), currentColor, currentLineIndex);
        m_textLinesWidth.push_back(curX + curW);
    }
}

#ifdef DEBUG_DRAW
void CUIXTextBlock::RenderUIDebugProperties()
{
    ImGui::Separator();
    ImGui::Text("Text Block");

    if (ImGui::Checkbox("Multiline", &m_bMultiline))
    {
        Rebuild();
    }
    ImGui::Text("Color");
    float color[4];
    color[0] = ((m_color >> 16) & 0xFF) / 255.0f; // R
    color[1] = ((m_color >> 8) & 0xFF) / 255.0f;  // G
    color[2] = ((m_color >> 0) & 0xFF) / 255.0f;  // B
    color[3] = ((m_color >> 24) & 0xFF) / 255.0f; // A
    if (ImGui::ColorEdit4("Tint", (float*)&color, ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB))
    {
        SetColor(u8(color[3] * 255.0f), u8(color[0] * 255.0f), u8(color[1] * 255.0f), u8(color[2] * 255.0f));
        Rebuild();
    }

    const char* alignHNames[] = { 
        "Left", "Center", "Right"//, "Fill"
    };
    int alignH = (int)m_eAlignH;
    if (ImGui::Combo("Horizontal Alignment", &alignH, alignHNames, IM_ARRAYSIZE(alignHNames)))
    {
        SetAlignHorz((EUIXAlignHorizontal)alignH);
        Rebuild();
    }

    const char* alignVNames[] = { 
        "Top", "Center", "Bottom"//, "Fill"
    };
    int alignV = (int)m_eAlignV;
    if (ImGui::Combo("Vertical Alignment", &alignV, alignVNames, IM_ARRAYSIZE(alignVNames)))
    {
        SetAlignVert((EUIXAlignVertical)alignV);
        //Rebuild();
    }

    
    ImGui::Separator();
    
    string1024 text;
    xr_strcpy(text, Platform::ANSI_TO_UTF8(m_DebugText.c_str()).c_str());
    if (ImGui::InputText("Content", text, IM_ARRAYSIZE(text)))
    {
        m_DebugText = text;
    }
    if (ImGui::Button("Apply Content")) 
    {
        SetText(Platform::UTF8_to_CP1251(m_DebugText.c_str()).c_str());
        Rebuild();
    }
    ImGui::Separator();

    string128 fontName;
    xr_strcpy(fontName, m_DebugFontName.c_str());
    if (ImGui::InputText("Font", fontName, IM_ARRAYSIZE(fontName)))
    {
        m_DebugFontName = fontName;
    }
    if (ImGui::Button("Apply Font")) 
    {
        SetFont(g_FontManager->GetFont(m_DebugFontName.c_str()));
        Rebuild();
    }
    
    inherited::RenderUIDebugProperties();
}
#endif

void CUIXTextBlock::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXTextBlock, CUIXWidget>("CUIXTextBlock")
            .def("Rebuild",         &CUIXTextBlock::Rebuild)
            .def("SetText",         &CUIXTextBlock::SetText)
            .def("GetText",         &CUIXTextBlock::GetText)
            .def("SetFont",         &CUIXTextBlock::SetFont)
            .def("GetFont",         &CUIXTextBlock::GetFont)
            .def("SetColor",        (void(CUIXTextBlock::*)(u8, u8, u8, u8))&CUIXTextBlock::SetColor)
            .def("SetColor",        (void(CUIXTextBlock::*)(const u32&))&CUIXTextBlock::SetColor)
            .def("GetColor",        &CUIXTextBlock::GetColor)
            .def("SetAlignVert",    &CUIXTextBlock::SetAlignVert)
            .def("GetAlignVert",    &CUIXTextBlock::GetAlignVert)
            .def("SetAlignHorz",    &CUIXTextBlock::SetAlignHorz)
            .def("GetAlignHorz",    &CUIXTextBlock::GetAlignHorz)
    ];
}

xr_string CUIXTextBlock::ReadAttr(const xr_string& tagBody, const xr_string& atrrName, const xr_string& defaultValue)
{
    size_t attr_pos = tagBody.find(atrrName);
    if (attr_pos == std::string_view::npos)
        return defaultValue;

    size_t val_start = attr_pos + atrrName.length();
    
    if (val_start + 2 > tagBody.size() || tagBody[val_start] != '=' || tagBody[val_start + 1] != '"')
        return defaultValue;
    
    val_start += 2; 

    size_t val_end = tagBody.find('"', val_start);
    if (val_end == std::string_view::npos)
        return defaultValue;

    return tagBody.substr(val_start, val_end - val_start);
}

void CUIXTextBlock::PushElement(u32 startIndex, u32 length, xr_vector2f position, u32 color, u32 currentLineIndex)
{
    //Msg("CUXTextBlock::Push element() '%s' at %f %f", m_pText.substr(startIndex, length).c_str(), position.x, position.y);
    m_glyphCount += length;

    SUIXTextElement element;
    element.StartIndex = startIndex;
    element.Length = length;
    element.Position = position;
    element.Color = color;
    element.lineIndex = currentLineIndex;
    m_Elements.push_back(element);
}

void CUIXTextBlock::NextLine(const u32& symbolIndex, u32& elementIndex, float& currentX, float& currentW, u32& currentLineIndex)
{
    elementIndex = symbolIndex + 1;
    currentX += currentW;
    currentW = 0.0f;
    if (currentX > GetRect().width() && m_bMultiline)
    {
        m_textLinesWidth.push_back(currentX);
        currentLineIndex++;
        currentX = 0.0f;
    }
}
