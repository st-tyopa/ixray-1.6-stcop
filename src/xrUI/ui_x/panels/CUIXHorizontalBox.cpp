#include "stdafx.h"
#include "CUIXHorizontalBox.h"
#include <luabind/luabind.hpp>

void CUIXHorizontalBoxSlot::Rebuild()
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
void CUIXHorizontalBoxSlot::RenderUIDebugProperties()
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

void CUIXHorizontalBoxSlot::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXHorizontalBoxSlot, CUIXWidgetSlot>("CUIXHorizontalBoxSlot")
    ];
}

CUIXHorizontalBox::~CUIXHorizontalBox()
{
    for (CUIXHorizontalBoxSlot* slot : m_slots)
    {
        xr_delete(slot);
    }
    inherited::~CUIXWidget();
}

void CUIXHorizontalBox::Draw()
{
    for (CUIXHorizontalBoxSlot* slot : m_slots)
    {
        slot->Draw();
    }
}

void CUIXHorizontalBox::Rebuild()
{
    const xr_rect_f currentRect = GetRect();
    const float boxWidth = currentRect.width();
    float totalFillWeight = 0.0f;
    float totalAutoWidth = 0.0f; 
    
    for (CUIXHorizontalBoxSlot* slot : m_slots)
    {
        if (slot->GetFillSpace())
        {
            totalFillWeight += slot->GetFillWeight();
        }
        else
        {
            totalAutoWidth += slot->GetDesiredSize().x + slot->GetPadding().x1 + slot->GetPadding().x2;
        }
    }

    float currentX = currentRect.x1;
    for (CUIXHorizontalBoxSlot* slot : m_slots)
    {
        float slotWidth;
        if (slot->GetFillSpace())
        {
            slotWidth = (boxWidth - totalAutoWidth) * slot->GetFillWeight() / totalFillWeight;
        }
        else
        {
            slotWidth = slot->GetDesiredSize().x + slot->GetPadding().x1 + slot->GetPadding().x2;
        }
        slot->SetRect(currentX, currentRect.y1, currentX + slotWidth, currentRect.y2);
        currentX += slotWidth;
        slot->Rebuild();
    }
}

CUIXWidgetSlot* CUIXHorizontalBox::AttachChild(CUIXWidget* widget)
{
    CUIXHorizontalBoxSlot* child = new CUIXHorizontalBoxSlot(this);
    m_slots.push_back(child);
    child->SetWidget(widget);
    return child;
}

#ifdef DEBUG_DRAW
void CUIXHorizontalBox::RenderUIDebugNodeChild()
{
    for (CUIXHorizontalBoxSlot* slot : m_slots)
    {
        slot->RenderUIDebugNode();
    }
}
#endif

void CUIXHorizontalBox::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXHorizontalBox, CUIXWidget>("CUIXHorizontalBox")
    ];
}
