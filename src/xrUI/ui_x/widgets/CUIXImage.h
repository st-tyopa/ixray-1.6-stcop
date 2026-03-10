#pragma once
#include "ui_x/common/CUIXWidget.h"
#include "../../xrScripts/script_export_space.h"

class CUIXBrush;

class CUIXImage : public CUIXWidget
{
    using inherited = CUIXWidget;
    CUIXBrush* m_pBrush;
public:
    // --- Constructors -----------------------------------------------
    CUIXImage(CUIXBrush* brush) : inherited("image"), m_pBrush(brush) {}
    ~CUIXImage() override;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXImage* ui_x_cast_image() override { return this; } 
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Draw() override;
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    CUIXBrush* GetBrush() const { return m_pBrush; }
    xr_vector2f GetDesiredSize() override;
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};
