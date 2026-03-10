#include "stdafx.h"
#include "CUIXBorder.h"
#include <luabind/luabind.hpp>

#include "ui_x/common/CUIXBrush.h"

void CUIXBorderSlot::Rebuild()
{
    if (!m_pWidget)
    {
        return;
    }

    const xr_rect_f parentRect = m_pParent->GetRect();
    xr_rect_f borderRect;

    borderRect.left     = parentRect.left   + m_padding.left;
    borderRect.right    = parentRect.right  - m_padding.right;
    borderRect.top      = parentRect.top    + m_padding.top;
    borderRect.bottom   = parentRect.bottom - m_padding.bottom;

    SetRect(borderRect);
    
    inherited::Rebuild();
}

#ifdef DEBUG_DRAW
void CUIXBorderSlot::RenderUIDebugProperties()
{
    ImVec4 padding = ImVec4(m_padding.x1, m_padding.y1, m_padding.x2, m_padding.y2);
    if (ImGui::DragFloat4("Padding", reinterpret_cast<float*>(&padding), 1.0f))
    {
        SetPadding(xr_rect_f().set(padding.x, padding.y, padding.z, padding.w));
    }
}
#endif

void CUIXBorderSlot::SetPadding(const xr_rect_f& padding, bool forceRebuild)
{
    m_padding = padding;
    if (forceRebuild)
    {
        Rebuild();
    }
}

void CUIXBorderSlot::SetPadding(float padding, bool forceRebuild)
{
    m_padding.set(padding, padding, padding, padding);
    if (forceRebuild)
    {
        Rebuild();
    }
}

void CUIXBorderSlot::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXBorderSlot, CUIXWidgetSlot>("CUIXBorderSlot")
            .def("SetPadding", (void(CUIXBorderSlot::*)(const xr_rect_f&, bool))&CUIXBorderSlot::SetPadding)
            .def("SetPadding", (void(CUIXBorderSlot::*)(float, bool))&CUIXBorderSlot::SetPadding)
            .def("GetPadding", &CUIXBorderSlot::GetPadding)
    ];
}

CUIXBorder::~CUIXBorder()
{
    if (m_pSlot)
    {
        xr_delete(m_pSlot);
    }
    if (m_pBrush)
    {
        xr_delete(m_pBrush);
    }
}

void CUIXBorder::Draw()
{
    if (m_pBrush)
    {
        m_pBrush->Render(GetRect(), m_renderTransform);
    }
    if (m_pSlot)
    {
        m_pSlot->Draw();
    }
}

void CUIXBorder::Rebuild()
{
    inherited::Rebuild();
    if (m_pSlot)
    {
        m_pSlot->Rebuild();
    }
}

CUIXWidgetSlot* CUIXBorder::AttachChild(CUIXWidget* widget)
{
    if (m_pSlot != nullptr)
    {
        return nullptr;
    }
    m_pSlot = new CUIXBorderSlot(this);
    m_pSlot->SetWidget(widget);
    return m_pSlot;
}

#ifdef DEBUG_DRAW
void CUIXBorder::RenderUIDebugNodeChild()
{
    if (m_pSlot)
    {
        m_pSlot->RenderUIDebugNode();
    }
}
#endif

void CUIXBorder::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXBorder, CUIXWidget>("CUIXBorder")
    ];
}
