#include "stdafx.h"
#include "CUIXSwitcher.h"
#include <luabind/luabind.hpp>

void CUIXSwitcherSlot::Rebuild()
{
    SetRect(m_pParent->GetRect());
    inherited::Rebuild();
}

void CUIXSwitcherSlot::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXSwitcherSlot, CUIXWidgetSlot>("CUIXSwitcherSlot")
    ];
}

CUIXSwitcher::~CUIXSwitcher()
{
    for (CUIXSwitcherSlot* slot : m_slots)
    {
        xr_delete(slot);
    }
    inherited::~CUIXWidget();
}

void CUIXSwitcher::Draw()
{
    if (!m_slots.empty())
    {
        m_slots[m_activeSlotIndex]->Draw();
    }
}

void CUIXSwitcher::Rebuild()
{
    inherited::Rebuild();
    for (CUIXSwitcherSlot* slot : m_slots)
    {
        slot->Rebuild();
    }
}

#ifdef DEBUG_DRAW
void CUIXSwitcher::RenderUIDebugNodeChild()
{
    for (CUIXSwitcherSlot* slot : m_slots)
    {
        slot->RenderUIDebugNode();
    }
}

void CUIXSwitcher::RenderUIDebugProperties()
{
    inherited::RenderUIDebugProperties();
    int activeIndex = m_activeSlotIndex;
    if (ImGui::DragInt("Active Index", &activeIndex, 1, 0, (int)(m_slots.size() - 1)))
    {
        m_activeSlotIndex = (u8)activeIndex;
    }
}   
#endif

void CUIXSwitcher::SetActiveSlotIndex(u8 index)
{
    if (m_slots.empty())
    {
        m_activeSlotIndex = 0;
        return;
    }
    if (index >= m_slots.size())
    {
        m_activeSlotIndex = (u8)(m_slots.size() - 1);
    }
    else
    {
        m_activeSlotIndex = index;
    }
}

bool CUIXSwitcher::SetActiveSlot(CUIXWidget* pWidget)
{
    if (!pWidget)
    {
        SetActiveSlotIndex(0);
        return true;
    }

    for (u8 i = 0; i < (u8)m_slots.size(); ++i)
    {
        if (m_slots[i]->GetWidget() == pWidget)
        {
            SetActiveSlotIndex(i);
            return true;
        }
    }
    return false;
}

CUIXWidgetSlot* CUIXSwitcher::AttachChild(CUIXWidget* widget)
{
    CUIXSwitcherSlot* child = new CUIXSwitcherSlot(this);
    m_slots.push_back(child);
    child->SetWidget(widget);
    return child;
}

bool CUIXSwitcher::OnMouseMove(int dx, int dy)
{
    if (m_eVisibility == EUIXVisibility::NonHit || inherited::OnMouseMove(dx, dy))
    {
        if (!m_slots.empty())
        {
            return m_slots[m_activeSlotIndex]->OnMouseMove(dx, dy);
        }   
    }
    return false;
}

bool CUIXSwitcher::OnKeyboardPressed(int key)
{
    if (inherited::OnKeyboardPressed(key))
    {
        return true;
    }
    if (!m_slots.empty())
    {
        return m_slots[m_activeSlotIndex]->OnKeyboardPressed(key);
    }
    return false;
}

void CUIXSwitcher::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXSwitcher, CUIXWidget>("CUIXSwitcher")
        .def("SetActiveSlotIndex",  &CUIXSwitcher::SetActiveSlotIndex)
        .def("GetActiveSlotIndex",  &CUIXSwitcher::GetActiveSlotIndex)
    ];
}
