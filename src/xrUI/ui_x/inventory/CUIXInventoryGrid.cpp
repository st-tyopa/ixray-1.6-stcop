#include "stdafx.h"
#include "CUIXInventoryGrid.h"

#include "ui_x/common/CUIXBrush.h"

CUIXInventoryGrid::CUIXInventoryGrid() : inherited("inventory_grid")
{
    m_gridSize = xr_vector2i().set(1, 1);
}

void CUIXInventoryGrid::Draw()
{
    if (m_pGridBrush != nullptr)
    {
        m_pGridBrush->Render(GetRect(), m_renderTransform);
    }
}

void CUIXInventoryGrid::Rebuild()
{
    if (m_pGridBrush != nullptr)
    {
        m_pGridBrush->SetSize(m_cellSize);
    }
    const xr_rect_f rect = GetRect();
    SetRect(rect.x1, rect.y1, rect.x1 + (float)m_gridSize.x * m_cellSize.x, rect.y1 + (float)m_gridSize.y * m_cellSize.y);
    inherited::Rebuild();
}

bool CUIXInventoryGrid::PushItem(CInventoryItem* pInvItem)
{
    return false;
}

bool CUIXInventoryGrid::PopItem(CInventoryItem* pInvItem)
{
    return false;
}

#ifdef DEBUG_DRAW
void CUIXInventoryGrid::RenderUIDebugProperties()
{
    if (m_pGridBrush)
    {
        m_pGridBrush->RenderUIDebugProperties();
    }
    
    inherited::RenderUIDebugProperties();
}
#endif
