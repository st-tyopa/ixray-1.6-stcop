#include "stdafx.h"
#include "CUIXBrush.h"
#include <luabind/luabind.hpp>

#include "UITextureMaster.h"
#include "ui_defs.h"
#include "../CUIXCore.h"
#include "../Include/xrRender/UIShader.h"

CUIXBrush::CUIXBrush(EUIXDrawType drawType)
{
    m_tint = 0xFFFFFFFF; // white default
    m_tile = EUIXTiling::none;
    m_drawType = drawType;

    CreateShader();
}

CUIXBrush::CUIXBrush(shared_str const& textureName, EUIXDrawType drawType)
{
    m_tint = 0xFFFFFFFF; // white default
    m_tile = EUIXTiling::none;
    m_drawType = drawType;

    CreateShader(textureName);
}

CUIXBrush::CUIXBrush(shared_str const& textureName, shared_str const& shaderName, EUIXDrawType drawType)
{
    m_tint = 0xFFFFFFFF; // white default
    m_tile = EUIXTiling::none;
    m_drawType = drawType;

    CreateShader(textureName, shaderName);   
}

void CUIXBrush::CreateShader(shared_str const& textureName, shared_str const& shaderName)
{
    if (m_drawType == EUIXDrawType::none)
    {
        return;
    }
    m_uv.set(0.0f, 0.0f, 1.0f, 1.0f);
    //SetSize(xr_vector2f().set(32.0f, 32.0f));

    string128 finalTextureName;
    if (m_drawType == EUIXDrawType::image)
    {
        xr_strconcat(finalTextureName, textureName.c_str());   
    }
    else
    {
        xr_strconcat(finalTextureName, textureName.c_str(), "_cc");
    }
    
    xr_rect_f textureRect;
    if (CUITextureMaster::InitTexture(finalTextureName, shaderName, m_shader, textureRect))
    {
        xr_vector2f fileSize;
        UIRender->GetTextureResolution(CUITextureMaster::GetTextureFileName(finalTextureName), fileSize);
        m_uv = textureRect;
        m_uv.div(fileSize.x, fileSize.y);
        SetSize(xr_vector2f().set(textureRect.width(), textureRect.height()));
    }
    else
    {
        UIRender->GetTextureResolution(finalTextureName, m_size);
    }

    CreateFrameShader(textureName, shaderName);
    
#ifdef DEBUG_DRAW
    m_DebugTextureName = textureName;
    m_DebugShaderName = shaderName;
#endif
}

void CUIXBrush::Render(const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform)
{
    
    if (m_drawType == EUIXDrawType::none)
    {
        return;
    }
    VERIFY						(g_bRendering);
    if (m_drawType == EUIXDrawType::image) // draw only default shader
    {
        UIRender->SetShader			(*m_shader);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(m_tile, drawRect, renderTransform);
        UIRender->FlushPrimitive	();
        return;
    }
    if (m_drawType == EUIXDrawType::border || m_drawType == EUIXDrawType::box) // draw frame
    {
        UIRender->SetShader			(*m_frameLeftTop);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::none, xr_rect_f{ drawRect.x1, drawRect.y1, drawRect.x1 + m_size.x, drawRect.y1 + m_size.y }, renderTransform);
        UIRender->FlushPrimitive	();
        
        UIRender->SetShader			(*m_frameLeftBottom);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::none, xr_rect_f{ drawRect.x1, drawRect.y2 - m_size.y, drawRect.x1 + m_size.x, drawRect.y2 }, renderTransform);
        UIRender->FlushPrimitive	();
        
        UIRender->SetShader			(*m_frameRightTop);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::none, xr_rect_f{ drawRect.x2 - m_size.x, drawRect.y1, drawRect.x2, drawRect.y1 + m_size.y }, renderTransform);
        UIRender->FlushPrimitive	();
        
        UIRender->SetShader			(*m_frameRightBottom);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::none, xr_rect_f{ drawRect.x2 - m_size.x, drawRect.y2 - m_size.y, drawRect.x2, drawRect.y2 }, renderTransform);
        UIRender->FlushPrimitive	();

        UIRender->SetShader			(*m_frameLeft);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::vertical, xr_rect_f{ drawRect.x1, drawRect.y1 + m_size.y, drawRect.x1 + m_size.x, drawRect.y2 - m_size.y }, renderTransform);
        UIRender->FlushPrimitive	();
        
        UIRender->SetShader			(*m_frameRight);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::vertical, xr_rect_f{ drawRect.x2 - m_size.x, drawRect.y1 + m_size.y, drawRect.x2, drawRect.y2 - m_size.y }, renderTransform);
        UIRender->FlushPrimitive	();
        
        UIRender->SetShader			(*m_frameTop);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::horizontal, xr_rect_f{ drawRect.x1 + m_size.x, drawRect.y1, drawRect.x2 - m_size.x, drawRect.y1 + m_size.y }, renderTransform);
        UIRender->FlushPrimitive	();
        
        UIRender->SetShader			(*m_frameBottom);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::horizontal, xr_rect_f{ drawRect.x1 + m_size.x, drawRect.y2 - m_size.y, drawRect.x2 - m_size.x, drawRect.y2 }, renderTransform);
        UIRender->FlushPrimitive	();
    }
    
    
    if (m_drawType == EUIXDrawType::box)
    {
        UIRender->SetShader			(*m_shader);
        UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
        RenderInternal				(EUIXTiling::both, xr_rect_f{ drawRect.x1 + m_size.x, drawRect.y1 + m_size.y, drawRect.x2 - m_size.x, drawRect.y2 - m_size.y }, renderTransform);
        UIRender->FlushPrimitive	();
    }
}

#ifdef DEBUG_DRAW
void CUIXBrush::RenderUIDebugProperties()
{
    ImGui::Separator();
    ImGui::Text("Brush");
    
    float color[4];
    color[0] = ((m_tint >> 16) & 0xFF) / 255.0f; // R
    color[1] = ((m_tint >> 8) & 0xFF) / 255.0f;  // G
    color[2] = ((m_tint >> 0) & 0xFF) / 255.0f;  // B
    color[3] = ((m_tint >> 24) & 0xFF) / 255.0f; // A
    if (ImGui::ColorEdit4("Tint", (float*)&color, ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB))
    {
        SetTint(u8(color[3] * 255.0f), u8(color[0] * 255.0f), u8(color[1] * 255.0f), u8(color[2] * 255.0f));
    }

    const char* tileNames[] = { 
        "None", "Vertical", "Horizontal", "Both"
    };
    int current_item = (int)m_tile;
    if (ImGui::Combo("Tiling", &current_item, tileNames, IM_ARRAYSIZE(tileNames)))
    {
        m_tile = (EUIXTiling)current_item;
    }

    if (m_tile != EUIXTiling::none)
    {
        xr_vector2f size = xr_vector2f().set(m_size.x, m_size.y);
        if (ImGui::DragFloat2("Brush Size", reinterpret_cast<float*>(&size), 1.0f))
        {
            m_size = size;
        }   
    }

    const char* drawTypeNames[] = { 
        "Image", "Border", "Box"
    };
    int current_drawType = (int)m_drawType;
    if (ImGui::Combo("Draw type", &current_drawType, drawTypeNames, IM_ARRAYSIZE(drawTypeNames)))
    {
        m_drawType = (EUIXDrawType)current_drawType;
        CreateShader(m_DebugTextureName, m_DebugShaderName);
    }
    
    string256 textureName;
    xr_strcpy(textureName, m_DebugTextureName.c_str());
    if (ImGui::InputText("Texture", textureName, IM_ARRAYSIZE(textureName)))
    {
        m_DebugTextureName = textureName;
    }
    string256 shaderName;
    xr_strcpy(shaderName, m_DebugShaderName.c_str());
    if (ImGui::InputText("Shader", shaderName, IM_ARRAYSIZE(shaderName)))
    {
        m_DebugShaderName = shaderName;
    }
    if (ImGui::Button("Apply Texture and Shader")) 
    {
        CreateShader(m_DebugTextureName, m_DebugShaderName);
    }
    ImGui::Separator();
}
#endif

void CUIXBrush::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<EUIXDrawType>("EDrawType")
            .enum_("constants")
            [
                value("Box",    EUIXDrawType::box),
                value("Border", EUIXDrawType::border),
                value("Image",  EUIXDrawType::image),
                value("None",   EUIXDrawType::none)
            ],
        class_<EUIXTiling>("ETiling")
            .enum_("constants")
            [
                value("None",       EUIXTiling::none),
                value("Vertical",   EUIXTiling::vertical),
                value("Horizontal", EUIXTiling::horizontal),
                value("Both",       EUIXTiling::both)
            ],
        class_<CUIXBrush>("CUIXBrush")
            .def("CreateShader", &CUIXBrush::CreateShader)
            .def("SetTint", (void (CUIXBrush::*)(u8, u8, u8, u8))&CUIXBrush::SetTint)
            .def("SetTint", (void (CUIXBrush::*)(u32))&CUIXBrush::SetTint)
            .def("GetTint", &CUIXBrush::GetTint)
            .def("SetSize", &CUIXBrush::SetSize)
            .def("GetSize", &CUIXBrush::GetSize)
            .def("SetTile", &CUIXBrush::SetTile)
            .def("GetTile", &CUIXBrush::GetTile)
            .def("SetDrawType", &CUIXBrush::SetDrawType)
            .def("GetDrawType", &CUIXBrush::GetDrawType)
    ];
}

void CUIXBrush::RenderInternal(const EUIXTiling tile, const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform) const
{    
    xr_vector2f uvLT = m_uv.lt;
    xr_vector2f uvRB;
    
    switch (tile)
    {
    case EUIXTiling::vertical:
        {
            uvRB.set(m_uv.x2, drawRect.height() / m_size.y);
        } break;
    case EUIXTiling::horizontal:
        {
            uvRB.set(drawRect.width() / m_size.x, m_uv.y2);  
        } break;
    case EUIXTiling::both:
        {
            uvRB.set(drawRect.width() / m_size.x, drawRect.height() / m_size.y);  
        } break;
    default:
        {
            uvRB = m_uv.rb;   
        } break;
        
    }

    const float cos_a = float(xr_cos(renderTransform.angle));
    const float sin_a = float(xr_sin(renderTransform.angle));
    
    const xr_vector2f scale = renderTransform.scale;
    const xr_vector2f pivot = xr_vector2f().set(renderTransform.pivot.x * drawRect.width() * scale.x, renderTransform.pivot.y * drawRect.height() * scale.y);
    const xr_vector2f lt = xr_vector2f().set(drawRect.lt.x - 0.5f, drawRect.lt.y - 0.5f);
    
    // clip poly
    CUIXPoly2dSafe poly;
    poly.resize(4);
    
    // LT
    poly[0].Set		    (0.0f,0.0f,uvLT.x,uvLT.y);
    poly[0].RotatePt	(pivot,cos_a,sin_a);
    poly[0].pt.add		(lt);

    // RT
    poly[1].Set		    (drawRect.width() * scale.x,0.0f,uvRB.x,uvLT.y);
    poly[1].RotatePt	(pivot,cos_a,sin_a);
    poly[1].pt.add		(lt);

    // RB
    poly[2].Set		    (drawRect.width() * scale.x,drawRect.height() * scale.y,uvRB.x,uvRB.y);
    poly[2].RotatePt	(pivot,cos_a,sin_a);
    poly[2].pt.add		(lt);

    // LB
    poly[3].Set		    (0.0f,drawRect.height() * scale.y,uvLT.x,uvRB.y);
    poly[3].RotatePt	(pivot,cos_a,sin_a);
    poly[3].pt.add		(lt);

    UIRender->PushPoint(poly[0].pt.x, poly[0].pt.y, 0, m_tint, poly[0].uv.x, poly[0].uv.y);
    UIRender->PushPoint(poly[1].pt.x, poly[1].pt.y, 0, m_tint, poly[1].uv.x, poly[1].uv.y);
    UIRender->PushPoint(poly[2].pt.x, poly[2].pt.y, 0, m_tint, poly[2].uv.x, poly[2].uv.y);
    UIRender->PushPoint(poly[0].pt.x, poly[0].pt.y, 0, m_tint, poly[0].uv.x, poly[0].uv.y);
    UIRender->PushPoint(poly[2].pt.x, poly[2].pt.y, 0, m_tint, poly[2].uv.x, poly[2].uv.y);
    UIRender->PushPoint(poly[3].pt.x, poly[3].pt.y, 0, m_tint, poly[3].uv.x, poly[3].uv.y);
}

void CUIXBrush::CreateFrameShader(shared_str const& textureName, shared_str const& shaderName)
{
    if (m_drawType == EUIXDrawType::border || m_drawType == EUIXDrawType::box)
    {
        xr_rect_f textureRect;
        string128 finalTextureName;
    
        xr_strconcat(finalTextureName, textureName.c_str(), "_lt");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameLeftTop, textureRect);
    
        xr_strconcat(finalTextureName, textureName.c_str(), "_lc");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameLeft, textureRect);
    
        xr_strconcat(finalTextureName, textureName.c_str(), "_lb");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameLeftBottom, textureRect);

        xr_strconcat(finalTextureName, textureName.c_str(), "_rt");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameRightTop, textureRect);
    
        xr_strconcat(finalTextureName, textureName.c_str(), "_rc");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameRight, textureRect);
    
        xr_strconcat(finalTextureName, textureName.c_str(), "_rb");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameRightBottom, textureRect);
    
        xr_strconcat(finalTextureName, textureName.c_str(), "_ct");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameTop, textureRect);
    
        xr_strconcat(finalTextureName, textureName.c_str(), "_cb");
        CUITextureMaster::InitTexture(finalTextureName, shaderName, m_frameBottom, textureRect);
    }
}
