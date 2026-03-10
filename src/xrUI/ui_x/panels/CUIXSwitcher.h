#pragma once
#include "ui_x/common/CUIXWidget.h"
#include "../../xrScripts/script_export_space.h"

class CUIXSwitcherSlot : public CUIXWidgetSlot
{
    using inherited = CUIXWidgetSlot;
public:
    // --- Constructors -----------------------------------------------
    CUIXSwitcherSlot(CUIXWidget* parent) : inherited("switcher_slot", parent) {}
    ~CUIXSwitcherSlot() override = default;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXSwitcherSlot* ui_x_cast_switcher_slot() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Rebuild() override;
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override {}
#endif
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};

class CUIXSwitcher: public CUIXWidget
{
    using inherited = CUIXWidget;
    u8 m_activeSlotIndex = 0;
    xr_vector<CUIXSwitcherSlot*> m_slots;
public:
    // --- Constructors -----------------------------------------------
    CUIXSwitcher() : inherited("switcher") {}
    ~CUIXSwitcher() override;
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Draw() override;
    void Rebuild() override;
    CUIXWidgetSlot* AttachChild(CUIXWidget* widget) override;
    // ----------------------------------------------------------------
    // --- Input ------------------------------------------------------
    bool OnMouseMove(int dx, int dy) override;
    bool OnKeyboardPressed(int key) override;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXSwitcher* ui_x_cast_switcher() override { return this; } 
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugNodeChild() override;
    void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
    void SetActiveSlotIndex(u8 index);
    bool SetActiveSlot(CUIXWidget* pWidget);
    u8 GetActiveSlotIndex() const { return m_activeSlotIndex; }
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};
