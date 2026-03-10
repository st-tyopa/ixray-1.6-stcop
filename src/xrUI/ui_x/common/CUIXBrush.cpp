#include "stdafx.h"
#include "CUIXBrush.h"
#include <luabind/luabind.hpp>

#include "UITextureMaster.h"
#include "ui_defs.h"
#include "../CUIXCore.h"
#include "../Include/xrRender/UIShader.h"

CUIXBrush::CUIXBrush()
{
    m_tint = 0xFFFFFFFF; // white default
    m_tile = EUIXTiling::none;
    m_drawType = EUIXDrawType::image;

    CreateShader();
}

CUIXBrush::CUIXBrush(shared_str const& textureName)
{
    m_tint = 0xFFFFFFFF; // white default
    m_tile = EUIXTiling::none;
    m_drawType = EUIXDrawType::image;

    CreateShader(textureName);
}

CUIXBrush::CUIXBrush(shared_str const& textureName, shared_str const& shaderName)
{
    m_tint = 0xFFFFFFFF; // white default
    m_tile = EUIXTiling::none;
    m_drawType = EUIXDrawType::image;

    CreateShader(textureName, shaderName);   
}

void CUIXBrush::CreateShader(shared_str const& textureName, shared_str const& shaderName)
{
    m_shader->create(shaderName.c_str(),textureName.c_str());
    
    xr_rect_f textureRect;
    if (CUITextureMaster::InitTexture(textureName, shaderName, m_shader, textureRect))
    {
        xr_vector2f fileSize;
        Msg("Loading texture from %s",CUITextureMaster::GetTextureFileName(textureName.c_str()));
        UIRender->GetTextureResolution(CUITextureMaster::GetTextureFileName(textureName.c_str()), fileSize);
        m_uv = textureRect;
        m_uv.div(fileSize.x, fileSize.y);
        SetSize(xr_vector2f().set(textureRect.width(), textureRect.height()));
    }
    else
    {
        m_uv.set(0.0f, 0.0f, 1.0f, 1.0f);
        SetSize(xr_vector2f().set(32.0f, 32.0f));
    }
    
#ifdef DEBUG_DRAW
    m_DebugTextureName = textureName;
    m_DebugShaderName = shaderName;
#endif
}

void CUIXBrush::Render(const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform)
{
    VERIFY						(g_bRendering);
    UIRender->SetShader			(*m_shader);
    UIRender->StartPrimitive	(6, IUIRender::ptTriList, IUIRender::pttTL);
    RenderInternal				(drawRect, renderTransform);
    UIRender->FlushPrimitive	();    
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
                value("None",   EUIXDrawType::none),
                value("Box",    EUIXDrawType::box),
                value("Border", EUIXDrawType::border),
                value("Image",  EUIXDrawType::image)
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

void CUIXBrush::RenderInternal(const xr_rect_f& drawRect, const FUIXRenderTransform& renderTransform)
{
    //xr_vector2f textureSize;
    //UIRender->GetActiveTextureResolution(textureSize);    
    
    xr_vector2f uvLT = m_uv.lt;
    xr_vector2f uvRB;
    
    switch (m_tile)
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
    

    const xr_vector2f pivot = xr_vector2f().set(renderTransform.pivot.x * drawRect.width(), renderTransform.pivot.y * drawRect.height());
    
    // clip poly
    CUIXPoly2dSafe poly;
    poly.resize(4);
    
    // LT
    poly[0].Set		    (0.0f,0.0f,uvLT.x,uvLT.y);
    poly[0].RotatePt	(pivot,cos_a,sin_a);
    poly[0].pt.add		(drawRect.lt);

    // RT
    poly[1].Set		    (drawRect.width(),0.0f,uvRB.x,uvLT.y);
    poly[1].RotatePt	(pivot,cos_a,sin_a);
    poly[1].pt.add		(drawRect.lt);

    // RB
    poly[2].Set		    (drawRect.width(),drawRect.height(),uvRB.x,uvRB.y);
    poly[2].RotatePt	(pivot,cos_a,sin_a);
    poly[2].pt.add		(drawRect.lt);

    // LB
    poly[3].Set		    (0.0f,drawRect.height(),uvLT.x,uvRB.y);
    poly[3].RotatePt	(pivot,cos_a,sin_a);
    poly[3].pt.add		(drawRect.lt);

    UIRender->PushPoint(poly[0].pt.x, poly[0].pt.y, 0, m_tint, poly[0].uv.x, poly[0].uv.y);
    UIRender->PushPoint(poly[1].pt.x, poly[1].pt.y, 0, m_tint, poly[1].uv.x, poly[1].uv.y);
    UIRender->PushPoint(poly[2].pt.x, poly[2].pt.y, 0, m_tint, poly[2].uv.x, poly[2].uv.y);
    UIRender->PushPoint(poly[0].pt.x, poly[0].pt.y, 0, m_tint, poly[0].uv.x, poly[0].uv.y);
    UIRender->PushPoint(poly[2].pt.x, poly[2].pt.y, 0, m_tint, poly[2].uv.x, poly[2].uv.y);
    UIRender->PushPoint(poly[3].pt.x, poly[3].pt.y, 0, m_tint, poly[3].uv.x, poly[3].uv.y);
}
