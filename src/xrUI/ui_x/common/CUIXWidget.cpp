#include "stdafx.h"
#include "CUIXWidget.h"

#include <json/json.hpp>
#include <luabind/luabind.hpp>

#include "ui_x/CUIXCore.h"

xr_atomic_u32 CUIXElement::m_idGenerator;

CUIXElement::CUIXElement(const shared_str& namePrefix)
{
    string256 temp;
    xr_sprintf( temp, "%s_%i", namePrefix.c_str(), m_idGenerator.fetch_add(1) );
    m_name = xr_string(temp);
}

CUIXElement::~CUIXElement()
{
#ifdef DEBUG_DRAW
    if (ui_x().m_pDebugUIElement == this)
    {
        ui_x().m_pDebugUIElement = nullptr;
    }
#endif
}

#ifdef DEBUG_DRAW
void CUIXElement::RenderUIDebugNode()
{
    ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_OpenOnArrow;// | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (ui_x().m_pDebugUIElement == this)
    {
        Flags |= ImGuiTreeNodeFlags_Selected;
    }
    const bool isOpen = ImGui::TreeNodeEx(this, Flags, "%s (%p)", m_name.c_str(), (void*)this);
    if (ImGui::IsItemClicked())
    {
        ui_x().m_pDebugUIElement = this;
    }
    if (isOpen)
    {
        RenderUIDebugNodeChild();
        ImGui::TreePop();   
    }
    
}

void CUIXElement::RenderUIDebugProperties()
{
    ImGui::Text("Not implemented");
}
#endif

void CUIXElement::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXElement>("CUIXElement")
            .def("SetName", &CUIXElement::SetName)
            .def("GetName", &CUIXElement::GetName)
            .def("GetRect", &CUIXElement::GetRect)
            .def("SetRect", (void (CUIXElement::*)(float, float, float, float))&CUIXElement::SetRect)
            .def("SetRect", (void (CUIXElement::*)(const xr_rect_f&))&CUIXElement::SetRect)
            .def("Rebuild", &CUIXElement::Rebuild)
    ];
}

void CUIXWidgetSlot::Rebuild()
{
    if (m_pWidget)
    {
        m_pWidget->SetRect(GetRect());
        m_pWidget->Rebuild();
    }
}

bool CUIXWidgetSlot::OnMouseMove(int dx, int dy)
{
    if (m_pWidget)
    {
        return m_pWidget->OnMouseMove(dx, dy);
    }
    return false;
}

bool CUIXWidgetSlot::OnKeyboardPressed(int key)
{
    if (m_pWidget)
    {
        return m_pWidget->OnKeyboardPressed(key);
    }
    return false;
}

bool CUIXWidget::OnMouseMove(int dx, int dy)
{
    if (m_eVisibility != EUIXVisibility::Visible)
    {
        return false;
    }
    xr_vector2f cursorPosition = ui_x().GetCursorPosition();
    if (GetRect().in(cursorPosition.x, cursorPosition.y))
    {
        for (auto& cb : m_mouseMoveCallbacks) {
            if (cb.is_valid())
            {
                cb(dx, dy);
            }
        }    
        return true;
    }
    return false;
}

bool CUIXWidget::OnKeyboardPressed(int key)
{
    for (auto& cb : m_keyboardPressedCallbacks) {
        if (cb.is_valid())
        {
            cb(key);
        }
    }
    return !m_keyboardPressedCallbacks.empty();
}

void CUIXWidget::OnFocusChanged(bool bIsFocused)
{
    for (auto& cb : m_focusChangedCallbacks) {
        if (cb.is_valid())
        {
            cb(bIsFocused);
        }
    }
}

#ifdef DEBUG_DRAW
void CUIXWidgetSlot::RenderUIDebugNodeChild()
{
    if (m_pWidget)
    {
        m_pWidget->RenderUIDebugNode();
    }
}

void CUIXWidgetSlot::RenderUIDebugPreviewFrame(u32 m_bDebugDrawPreviewColor)
{
    if (m_bDebugDrawPreviewFrame && m_pWidget)
    {
        ImDrawList* Draw = ImGui::GetForegroundDrawList();
        Draw->AddRect(
            ImVec2(m_pWidget->GetRect().x1, m_pWidget->GetRect().y1),
            ImVec2(m_pWidget->GetRect().x2, m_pWidget->GetRect().y2),
            m_bDebugDrawPreviewColor, 0.0f, 0, 1.0f);
    }
}

void CUIXWidget::RenderUIDebugProperties()
{
    ImGui::Separator();
    ImGui::Text("Render Transform");

    xr_vector2f pivot = xr_vector2f().set(m_renderTransform.pivot.x, m_renderTransform.pivot.y);
    if (ImGui::DragFloat2("Pivot", reinterpret_cast<float*>(&pivot), 0.1f, 0.0f, 1.0f))
    {
        m_renderTransform.pivot = pivot;
    }
    
    float angle = 180.0f * m_renderTransform.angle / PI;
    if (ImGui::DragFloat("Angle", &angle, 1.0f, -180.0f, 180.0f))
    {
        clamp(angle, -180.0f, 180.0f);
        m_renderTransform.angle = angle * PI / 180.0f;
    }

    xr_vector2f scale = xr_vector2f().set(m_renderTransform.scale.x, m_renderTransform.scale.y);
    if (ImGui::DragFloat2("Scale", reinterpret_cast<float*>(&scale), 0.1f))
    {
        m_renderTransform.scale = scale;
    }
}
#endif

void CUIXWidgetSlot::SetWidget(CUIXWidget* widget)
{
    if (m_pWidget != nullptr)
    {
        return;
    }
    m_pWidget = widget;
    m_pWidget->SetParentSlot(this);
}

CUIXWidgetSlot::~CUIXWidgetSlot()
{
    xr_delete(m_pWidget);
    inherited::~CUIXElement();
}

void CUIXWidgetSlot::Draw()
{
    if (m_pWidget)
    {
        switch (m_pWidget->GetVisibility())
        {
            case EUIXVisibility::Visible:
            case EUIXVisibility::NonHit:
            case EUIXVisibility::NonHitWithChild:
                m_pWidget->Draw();
                return;
            default:
                return;
        }
    }
}

xr_vector2f CUIXWidgetSlot::GetDesiredSize()
{
    if (m_pWidget != nullptr)
    {
        return m_pWidget->GetDesiredSize();
    }
    return CUIXElement::GetDesiredSize();
}

void CUIXWidgetSlot::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXWidgetSlot, CUIXElement>("CUIXWidgetSlot")
            .def("SetWidget", &CUIXWidgetSlot::SetWidget)
            .def("GetWidget", &CUIXWidgetSlot::GetWidget)
            .def("GetParent", &CUIXWidgetSlot::GetParent)
    ];
}

void CUIXWidget::SetName(const shared_str& name)
{
    if (!GetName().empty() && g_uiXCore)
    {
        g_uiXCore->UnregisterWidgetId(GetName().c_str());
    }
    inherited::SetName(name.c_str()); 
    if (!GetName().empty() && g_uiXCore)
    {
        g_uiXCore->RegisterWidgetId(GetName().c_str(), this);
    }
}

void CUIXWidget::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXWidget, CUIXElement>("CUIXWidget")
            .def("AttachChild", &CUIXWidget::AttachChild)
            .def("SetParentSlot", &CUIXWidget::SetParentSlot)
            .def("GetParentSlot", &CUIXWidget::GetParentSlot)
            .def("SetVisibility", &CUIXWidget::SetVisibility)
            .def("GetVisibility", &CUIXWidget::GetVisibility)
            .def("SetRenderTransform", &CUIXWidget::SetRenderTransform)
            .def("GetRenderTransform", &CUIXWidget::GetRenderTransform)
            .def("BindMouseMoveCallback", &CUIXWidget::BindMouseMoveCallback)
            .def("BindKeyboardPressedCallback", &CUIXWidget::BindKeyboardPressedCallback)
            .def("BindFocusChangedCallback", &CUIXWidget::BindFocusChangedCallback)
    ];
}

void CUIXWidget::BindMouseMoveCallback(const luabind::functor<void>& func)
{
    m_mouseMoveCallbacks.push_back(func);
}

void CUIXWidget::BindKeyboardPressedCallback(const luabind::functor<void>& func)
{
    m_keyboardPressedCallbacks.push_back(func);
}

void CUIXWidget::BindFocusChangedCallback(const luabind::functor<void>& func)
{
    m_focusChangedCallbacks.push_back(func);
}


