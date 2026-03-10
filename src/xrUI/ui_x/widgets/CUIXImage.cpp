#include "stdafx.h"
#include "CUIXImage.h"
#include <luabind/luabind.hpp>

#include "ui_x/common/CUIXBrush.h"

CUIXImage::~CUIXImage()
{
    xr_delete(m_pBrush);
}

void CUIXImage::Draw()
{
    if (m_pBrush)
    {
        m_pBrush->Render(GetRect(), m_renderTransform);
    }
}

#ifdef DEBUG_DRAW
void CUIXImage::RenderUIDebugProperties()
{
    if (m_pBrush)
    {
        m_pBrush->RenderUIDebugProperties();
    }
    
    inherited::RenderUIDebugProperties();
}
#endif

xr_vector2f CUIXImage::GetDesiredSize()
{
    if (m_pBrush)
    {
        return m_pBrush->GetSize();
    }
    return inherited::GetDesiredSize();
}

void CUIXImage::script_register(lua_State *L)
{
    using namespace luabind;

    module(L)
    [
        class_<CUIXImage, CUIXWidget>("CUIXImage")
            .def("GetBrush", &CUIXImage::GetBrush)
    ];
}
