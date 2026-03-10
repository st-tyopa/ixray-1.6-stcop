#include "stdafx.h"
#include "CUIXVerticalBox.h"
#include <luabind/luabind.hpp>

void CUIXVerticalBoxSlot::Rebuild()
{
    if (m_pWidget == nullptr)
    {
        return;
    }
    const xr_rect_f rect = GetRect();
    m_pWidget->SetRect(rect.x1 + m_padding.x1, rect.y1 + m_padding.y1, rect.x2 - m_padding.x2, rect.y2 - m_padding.y2);
    m_pWidget->Rebuild();
}

#ifdef DEBUG_DRAW
void CUIXVerticalBoxSlot::RenderUIDebugProperties()
{
    ImVec4 padding = ImVec4(m_padding.x1, m_padding.y1, m_padding.x2, m_padding.y2);
    if (ImGui::DragFloat4("Padding", reinterpret_cast<float*>(&padding), 1.0f))
    {
        SetPadding(xr_rect_f().set(padding.x, padding.y, padding.z, padding.w));
        GetParent()->Rebuild();
    }
    if (ImGui::Checkbox("Fill Spacing", &m_bFillSpace))
    {
        GetParent()->Rebuild();
    }
    if (m_bFillSpace)
    {
        if (ImGui::DragFloat("Fill Weight", &m_fillWeight, 0.1f))
        {
            GetParent()->Rebuild();
        }   
    }
}
#endif

void CUIXVerticalBoxSlot::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXVerticalBoxSlot, CUIXWidgetSlot>("CUIXVerticalBoxSlot")
    ];
}

CUIXVerticalBox::~CUIXVerticalBox()
{
    for (CUIXVerticalBoxSlot* slot : m_slots)
    {
        xr_delete(slot);
    }
    inherited::~CUIXWidget();
}

void CUIXVerticalBox::Draw()
{
    for (CUIXVerticalBoxSlot* slot : m_slots)
    {
        slot->Draw();
    }
}

void CUIXVerticalBox::Rebuild()
{
    const xr_rect_f currentRect = GetRect();
    const float boxHeight = currentRect.height();
    float totalFillWeight = 0.0f;
    float totalAutoHeight = 0.0f; 
    
    for (CUIXVerticalBoxSlot* slot : m_slots)
    {
        if (slot->GetFillSpace())
        {
            totalFillWeight += slot->GetFillWeight();
        }
        else
        {
            totalAutoHeight += slot->GetDesiredSize().y + slot->GetPadding().y1 + slot->GetPadding().y2;
        }
    }

    float currentY = currentRect.y1;
    for (CUIXVerticalBoxSlot* slot : m_slots)
    {
        float slotHeight;
        if (slot->GetFillSpace())
        {
            slotHeight = (boxHeight - totalAutoHeight) * slot->GetFillWeight() / totalFillWeight;
        }
        else
        {
            slotHeight = slot->GetDesiredSize().y + slot->GetPadding().y1 + slot->GetPadding().y2;
        }
        slot->SetRect(currentRect.x1, currentY, currentRect.x2, currentY + slotHeight);
        currentY += slotHeight;
        slot->Rebuild();
    }
}

CUIXWidgetSlot* CUIXVerticalBox::AttachChild(CUIXWidget* widget)
{
    CUIXVerticalBoxSlot* child = new CUIXVerticalBoxSlot(this);
    m_slots.push_back(child);
    child->SetWidget(widget);
    return child;
}

#ifdef DEBUG_DRAW
void CUIXVerticalBox::RenderUIDebugNodeChild()
{
    for (CUIXVerticalBoxSlot* slot : m_slots)
    {
        slot->RenderUIDebugNode();
    }
}
#endif

void CUIXVerticalBox::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXVerticalBox, CUIXWidget>("CUIXVerticalBox")
    ];
}
