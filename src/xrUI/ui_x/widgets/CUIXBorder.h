#pragma once
#include "ui_x/common/CUIXWidget.h"
#include "../../xrScripts/script_export_space.h"

class CUIXBrush;

class CUIXBorderSlot : public CUIXWidgetSlot
{
    using inherited = CUIXWidgetSlot;
    xr_rect_f m_padding;
public:
    // --- Constructors -----------------------------------------------
    CUIXBorderSlot(CUIXWidget *parent) : inherited("border_slot", parent) {}
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXBorderSlot* ui_x_cast_border_slot() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Rebuild() override;
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    void SetPadding(const xr_rect_f& padding, bool forceRebuild = true);
    void SetPadding(float padding, bool forceRebuild = true);
    const xr_rect_f& GetPadding() const { return m_padding; }
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};

class CUIXBorder : public CUIXWidget
{
    using inherited = CUIXWidget;
    CUIXBorderSlot* m_pSlot;
    CUIXBrush* m_pBrush;
public:
    // --- Constructors -----------------------------------------------
    CUIXBorder(CUIXBrush* brush) : inherited("border"), m_pSlot(nullptr), m_pBrush(brush) {}
    ~CUIXBorder() override;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXBorder* ui_x_cast_border() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Draw() override;
    void Rebuild() override;
    CUIXWidgetSlot* AttachChild(CUIXWidget* widget) override;
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugNodeChild() override;
#endif
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};
