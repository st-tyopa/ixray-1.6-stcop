#include "stdafx.h"
#include "CUIXImage.h"

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
