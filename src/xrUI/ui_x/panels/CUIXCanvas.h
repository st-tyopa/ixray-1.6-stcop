#pragma once
#include "ui_x/UIXDefs.h"
#include "ui_x/common/CUIXWidget.h"

class CUIXWidget;

enum class EUIXAnchor : u8
{
    ltAnchor,
    lcAnchor,
    lbAnchor,
    ctAnchor,
    ccAnchor,
    cbAnchor,
    rtAnchor,
    rcAnchor,
    rbAnchor,
    ftAnchor,
    fcAnchor,
    fbAnchor,
    lfAnchor,
    cfAnchor,
    rfAnchor,
    ffAnchor,
};

class CUIXCanvasSlot : public CUIXWidgetSlot
{
    using inherited = CUIXWidgetSlot;
protected:
    xr_vector2f m_size;
    xr_vector2f m_position;
    EUIXAnchor m_anchor = EUIXAnchor::ltAnchor;
    xr_vector2f m_alignment;

public:
    CUIXCanvasSlot(CUIXWidget* parent) : inherited("canvas_slot", parent) {}
    ~CUIXCanvasSlot() override = default;
    
    void Rebuild() override;

#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif

    void            SetAnchor       (EUIXAnchor anchor, bool forceRebuild = true);
    EUIXAnchor      GetAnchor       () const { return m_anchor; }
    
    void            SetAlignment    (const xr_vector2f& alignment, bool forceRebuild = true);
    xr_vector2f     GetAlignment    () const { return m_alignment; }

    void            SetPosition     (const xr_vector2f& position, bool forceRebuild = true);
    xr_vector2f     GetPosition     () const { return m_position; }
    
    void            SetSize         (const xr_vector2f& size, bool forceRebuild = true);
    xr_vector2f     GetSize         () const { return m_size; }
};

class CUIXCanvas : public CUIXWidget
{
    using inherited = CUIXWidget;
    xr_list<CUIXCanvasSlot*> m_slots;
public:
    CUIXCanvas() : inherited("canvas") {}
    ~CUIXCanvas() override;
    
    CUIXCanvas* ui_x_cast_canvas() override { return this; } 

    void Draw() override;
    void Rebuild() override;

#ifdef DEBUG_DRAW
    void RenderUIDebugNodeChild() override;
#endif

    CUIXCanvasSlot* AttachChild(CUIXWidget* widget);
};
