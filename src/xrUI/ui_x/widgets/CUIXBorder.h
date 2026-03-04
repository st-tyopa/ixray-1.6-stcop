#pragma once
#include "ui_x/common/CUIXWidget.h"

class CUIXBrush;

class CUIXBorderSlot : public CUIXWidgetSlot
{
    using inherited = CUIXWidgetSlot;
    xr_rect_f m_padding;
public:
    CUIXBorderSlot(CUIXWidget *parent) : inherited("border_slot", parent) {}
    
    void Rebuild() override;

#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif

    void SetPadding(const xr_rect_f& padding, bool forceRebuild = true);
    void SetPadding(float padding, bool forceRebuild = true);
    const xr_rect_f& GetPadding() const { return m_padding; }
};

class CUIXBorder : public CUIXWidget
{
    using inherited = CUIXWidget;
    CUIXBorderSlot* m_pSlot;
    CUIXBrush* m_pBrush;
public:
    CUIXBorder(CUIXBrush* brush) : inherited("border"), m_pSlot(nullptr), m_pBrush(brush) {}
    ~CUIXBorder() override;

    void Draw() override;
    void Rebuild() override;

#ifdef DEBUG_DRAW
    void RenderUIDebugNodeChild() override;
#endif
    /** 
     * Try to attach child widget
     * @widget to attach
     * @return slot or null if child already exist  
     */
    CUIXBorderSlot* AttachChild(CUIXWidget* widget);
};
