#include "stdafx.h"
#include "CUIXCanvas.h"
#include <luabind/luabind.hpp>

void CUIXCanvasSlot::Rebuild()
{
    if (!m_pWidget)
    {
        return;
    }    
    const xr_vector2f offset = xr_vector2f().set(m_position.x - m_alignment.x * m_size.x, m_position.y - m_alignment.y * m_size.y);
    const xr_rect_f parentRect = m_pParent->GetRect();
    xr_vector2f position;
    xr_vector2f size;
    
    // build widget position
    switch (m_anchor)
    {
        case EUIXAnchor::ltAnchor:
        case EUIXAnchor::lcAnchor:
        case EUIXAnchor::lbAnchor:
        case EUIXAnchor::lfAnchor:
        case EUIXAnchor::ftAnchor:
        case EUIXAnchor::fcAnchor:
        case EUIXAnchor::fbAnchor:
        case EUIXAnchor::ffAnchor:
            {
                position.x = offset.x + parentRect.left;
            } break;
        case EUIXAnchor::ctAnchor:
        case EUIXAnchor::ccAnchor:
        case EUIXAnchor::cbAnchor:
        case EUIXAnchor::cfAnchor:
            {
                xr_vector2f center;
                parentRect.getcenter(center);
                position.x = offset.x + center.x;
            } break;
        case EUIXAnchor::rtAnchor:
        case EUIXAnchor::rcAnchor:
        case EUIXAnchor::rbAnchor:
        case EUIXAnchor::rfAnchor:
            {
                position.x = offset.x + parentRect.right;
            } break;
    }
    switch (m_anchor)
    {
        case EUIXAnchor::ltAnchor:
        case EUIXAnchor::ctAnchor:
        case EUIXAnchor::rtAnchor:
        case EUIXAnchor::ftAnchor:
        case EUIXAnchor::lfAnchor:
        case EUIXAnchor::cfAnchor:
        case EUIXAnchor::rfAnchor:
        case EUIXAnchor::ffAnchor:
            {
                position.y = offset.y + parentRect.top;
            } break;
        case EUIXAnchor::lcAnchor:
        case EUIXAnchor::ccAnchor:
        case EUIXAnchor::rcAnchor:
        case EUIXAnchor::fcAnchor:
            {
                xr_vector2f center;
                parentRect.getcenter(center);
                position.y = offset.y + center.y;
            } break;
        case EUIXAnchor::lbAnchor:
        case EUIXAnchor::cbAnchor:
        case EUIXAnchor::rbAnchor:
        case EUIXAnchor::fbAnchor:
            {
                position.y = offset.y + parentRect.bottom;
            } break;
    }

    // build widget size
    switch (m_anchor)
    {
        case EUIXAnchor::ftAnchor:
        case EUIXAnchor::fcAnchor:
        case EUIXAnchor::fbAnchor:
            {
                size.x = parentRect.width();
            } break;
        case EUIXAnchor::lfAnchor:
        case EUIXAnchor::cfAnchor:
        case EUIXAnchor::rfAnchor:
            {
                size.y = parentRect.height();
            } break;
        case EUIXAnchor::ffAnchor:
            {
                size.x = parentRect.width();
                size.y = parentRect.height();
            } break;
        default:
            {
                size.x = m_size.x;
                size.y = m_size.y;
            }
    }    
    // setup
    m_pWidget->SetRect(position.x, position.y, position.x + size.x, position.y + size.y);
    m_pWidget->Rebuild();
}

void CUIXCanvasSlot::SetAnchor(EUIXAnchor anchor, bool forceRebuild)
{
    m_anchor = anchor;
    if (forceRebuild)
    {
        Rebuild();   
    }
}

void CUIXCanvasSlot::SetAlignment(const xr_vector2f& alignment, bool forceRebuild)
{
    m_alignment = alignment;
    if (forceRebuild)
    {
        Rebuild();   
    }
}

void CUIXCanvasSlot::SetPosition(const xr_vector2f& position, bool forceRebuild)
{
    m_position = position;
    if (forceRebuild)
    {
        Rebuild();
    }
}

void CUIXCanvasSlot::SetSize(const xr_vector2f& size, bool forceRebuild)
{
    m_size = size;
    if (forceRebuild)
    {
        Rebuild();
    }
}

#ifdef DEBUG_DRAW
void CUIXCanvasSlot::RenderUIDebugProperties()
{
    xr_vector2f position = xr_vector2f().set(m_position.x, m_position.y);
    if (ImGui::DragFloat2("Position", reinterpret_cast<float*>(&position), 1.0f))
    {
        SetPosition(position);
    }

    xr_vector2f size = xr_vector2f().set(m_size.x, m_size.y);
    if (ImGui::DragFloat2("Size", reinterpret_cast<float*>(&size), 1.0f))
    {
        SetSize(size);
    }

    xr_vector2f alignment = xr_vector2f().set(m_alignment.x, m_alignment.y);
    if (ImGui::DragFloat2("Alignment", reinterpret_cast<float*>(&alignment), 0.1f))
    {
        SetAlignment(alignment);
    }

    const char* anchorNames[] = { 
        "Left Top", "Left Center", "Left Bottom", 
        "Center Top", "Center Center", "Center Bottom",
        "Right Top", "Right Center", "Right Bottom",
        "Full Top", "Full Center", "Full Bottom",
        "Left Full", "Center Full", "Right Full",
        "Full Full" 
    };
    int current_item = (int)m_anchor;
    if (ImGui::Combo("Anchor", &current_item, anchorNames, IM_ARRAYSIZE(anchorNames)))
    {
        SetAnchor((EUIXAnchor)current_item);
    }
}
#endif

void CUIXCanvasSlot::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<EUIXAnchor>("EUIXAnchor")
            .enum_("constants")
            [
                value("lt", EUIXAnchor::ltAnchor), value("lc", EUIXAnchor::lcAnchor), value("lb", EUIXAnchor::lbAnchor),
                value("ct", EUIXAnchor::ctAnchor), value("cc", EUIXAnchor::ccAnchor), value("cb", EUIXAnchor::cbAnchor),
                value("rt", EUIXAnchor::rtAnchor), value("rc", EUIXAnchor::rcAnchor), value("rb", EUIXAnchor::rbAnchor),
                value("ft", EUIXAnchor::ftAnchor), value("fc", EUIXAnchor::fcAnchor), value("fb", EUIXAnchor::fbAnchor),
                value("lf", EUIXAnchor::lfAnchor), value("cf", EUIXAnchor::cfAnchor), value("rf", EUIXAnchor::rfAnchor),
                value("ff", EUIXAnchor::ffAnchor)
            ],
        class_<CUIXCanvasSlot, CUIXWidgetSlot>("CUIXCanvasSlot")
            .def("SetAnchor",    &CUIXCanvasSlot::SetAnchor)
            .def("GetAnchor",    &CUIXCanvasSlot::GetAnchor)
                
            .def("SetAlignment", &CUIXCanvasSlot::SetAlignment)
            .def("GetAlignment", &CUIXCanvasSlot::GetAlignment)
                
            .def("SetPosition",  &CUIXCanvasSlot::SetPosition)
            .def("GetPosition",  &CUIXCanvasSlot::GetPosition)
                
            .def("SetSize",      &CUIXCanvasSlot::SetSize)
            .def("GetSize",      &CUIXCanvasSlot::GetSize)
    ];
}

CUIXCanvas::~CUIXCanvas()
{
    for (CUIXCanvasSlot* slot : m_slots)
    {
        xr_delete(slot);
    }
    inherited::~CUIXWidget();
}

void CUIXCanvas::Draw()
{
    for (CUIXCanvasSlot* slot : m_slots)
    {
        slot->Draw();
    }
}

void CUIXCanvas::Rebuild()
{
    inherited::Rebuild();
    for (CUIXCanvasSlot* slot : m_slots)
    {
        slot->Rebuild();
    }
}

CUIXWidgetSlot* CUIXCanvas::AttachChild(CUIXWidget* widget)
{
    CUIXCanvasSlot* child = new CUIXCanvasSlot(this);
    m_slots.push_back(child);
    child->SetWidget(widget);
    return child;
}

bool CUIXCanvas::OnMouseMove(int dx, int dy)
{
    if (m_eVisibility == EUIXVisibility::NonHit || inherited::OnMouseMove(dx, dy))
    {
        auto it = m_slots.rbegin();
        auto it_end = m_slots.rend();
        for (; it != it_end; ++it)
        {
            if ((*it)->OnMouseMove(dx, dy))
            {
                return true;
            }   
        }
    }
    return false;
}

bool CUIXCanvas::OnKeyboardPressed(int key)
{
    if (inherited::OnKeyboardPressed(key))
    {
        return true;
    }
    for (CUIXCanvasSlot* slot : m_slots)
    {
        if (slot->OnKeyboardPressed(key))
        {
            return true;
        }
    }
    return false;
}

#ifdef DEBUG_DRAW
void CUIXCanvas::RenderUIDebugNodeChild()
{
    for (CUIXCanvasSlot* slot : m_slots)
    {
        slot->RenderUIDebugNode();
    }
}
#endif

void CUIXCanvas::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXCanvas, CUIXWidget>("CUIXCanvas")
    ];
}
