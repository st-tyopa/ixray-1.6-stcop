#pragma once
#include "ui_x/common/CUIXWidget.h"

class CUIXVerticalBoxSlot : public CUIXWidgetSlot
{
    using inherited = CUIXWidgetSlot;
protected:
    xr_rect_f m_padding;
    bool m_bFillSpace = false;
    float m_fillWeight = 1.0f;
public:
    // --- Constructors -----------------------------------------------
    CUIXVerticalBoxSlot(CUIXWidget* parent) : inherited("vert_box_slot", parent) {}
    ~CUIXVerticalBoxSlot() override = default;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXVerticalBoxSlot* ui_x_cast_vertical_box_slot() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Rebuild() override;
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    void SetPadding(const xr_rect_f& padding) { m_padding = padding; }
    void SetPadding(const float padding) { m_padding.set(padding, padding, padding, padding); }
    xr_rect_f GetPadding() const { return m_padding; }

    void SetFillSpace(bool fillSpace) { m_bFillSpace = fillSpace; }
    bool GetFillSpace() const { return m_bFillSpace; }

    void SetFillWeight(float fillWeight) { m_fillWeight = fillWeight; }
    float GetFillWeight() const { return m_fillWeight; }
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};

class CUIXVerticalBox : public CUIXWidget
{
    using inherited = CUIXWidget;
protected:
    xr_list<CUIXVerticalBoxSlot*> m_slots;
public:
    // --- Constructors -----------------------------------------------
    CUIXVerticalBox() : inherited("vertical_box") {}
    ~CUIXVerticalBox() override;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXVerticalBox* ui_x_cast_vertical_box() override { return this; }
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
