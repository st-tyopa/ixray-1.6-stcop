#pragma once
#include "ui_x/UIXDefs.h"
#include "ui_x/common/CUIXWidget.h"

class CUIXBrush;

class CUIXImage : public CUIXWidget
{
    using inherited = CUIXWidget;
    CUIXBrush* m_pBrush;
public:
    CUIXImage(CUIXBrush* brush) : inherited("image"), m_pBrush(brush) {}
    ~CUIXImage() override;

    CUIXImage* ui_x_cast_image() override { return this; } 

    void Draw() override;

#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif

    CUIXBrush* GetBrush() const { return m_pBrush; }
};
