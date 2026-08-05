#pragma once
#include "ui_x/common/CUIXWidget.h"


class CUIXScaleBoxSlot : public CUIXWidgetSlot
{
    using inherited = CUIXWidgetSlot;
public:
    // --- Constructors -----------------------------------------------
    CUIXScaleBoxSlot(CUIXWidget* parent) : inherited("scale_box_slot", parent) {}
    ~CUIXScaleBoxSlot() override = default;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXScaleBoxSlot* ui_x_cast_scale_box_slot() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Rebuild() override;
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    
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

class CUIXScaleBox : public CUIXWidget
{
    using inherited = CUIXWidget;  
public:
    // --- Constructors -----------------------------------------------
    CUIXScaleBox() : inherited("scale_box") {}
    ~CUIXScaleBox() override = default;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXScaleBox* ui_x_cast_scale_box() override { return this; }
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

