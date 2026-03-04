#include "stdafx.h"
#include "CUIXWidget.h"

#include "imgui_internal.h"
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

void CUIXWidgetSlot::Rebuild()
{
    if (m_pWidget)
    {
        m_pWidget->SetRect(GetRect());
        m_pWidget->Rebuild();
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
}
#endif

CUIXWidgetSlot::~CUIXWidgetSlot()
{
    xr_delete(m_pWidget);
    inherited::~CUIXElement();
}

void CUIXWidgetSlot::Draw()
{
    if (m_pWidget)
    {
        m_pWidget->Draw();
    }
}
