#pragma once
#include "ui_x/common/CUIXWidget.h"

class CUIXBrush;
class CInventoryItem;

class CUIXInventoryItem : public CUIXWidget
{
    using inherited = CUIXWidget;

    CInventoryItem* m_pInvItem;
    CUIXBrush* m_pBackgroundBrush;
    CUIXBrush* m_pItemBrush;
public:
    // --- Constructors -----------------------------------------------
    CUIXInventoryItem(CInventoryItem* invItem);
    ~CUIXInventoryItem() override = default;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXInventoryItem*  ui_x_cast_inventory_item() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Draw() override;
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    //void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    xr_vector2f GetDesiredSize() override;
    // ----------------------------------------------------------------
    
};
