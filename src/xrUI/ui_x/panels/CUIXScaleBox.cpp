#include "stdafx.h"
#include "CUIXScaleBox.h"


void CUIXScaleBoxSlot::Rebuild()
{
    CUIXWidgetSlot::Rebuild();
}

#ifdef DEBUG_DRAW
void CUIXScaleBoxSlot::RenderUIDebugProperties()
{
    CUIXWidgetSlot::RenderUIDebugProperties();
}
#endif

void CUIXScaleBoxSlot::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXScaleBoxSlot, CUIXWidgetSlot>("CUIXScaleBoxSlot")
    ];
}

void CUIXScaleBox::Draw()
{
    
}

void CUIXScaleBox::Rebuild()
{
    
}

CUIXWidgetSlot* CUIXScaleBox::AttachChild(CUIXWidget* widget)
{
    return CUIXWidget::AttachChild(widget);
}

#ifdef DEBUG_DRAW
void CUIXScaleBox::RenderUIDebugNodeChild()
{
    CUIXWidget::RenderUIDebugNodeChild();
}
#endif

void CUIXScaleBox::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXScaleBox, CUIXWidget>("CUIXScaleBox")
    ];
}
