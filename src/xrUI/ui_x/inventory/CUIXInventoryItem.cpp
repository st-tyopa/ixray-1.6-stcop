#include "stdafx.h"
#include "CUIXInventoryItem.h"

//#include "../../../xrGame/inventory_item.h"
#include "ui_x/common/CUIXBrush.h"

CUIXInventoryItem::CUIXInventoryItem(CInventoryItem* invItem) : inherited("inv_item")
{
    /*
    m_pInvItem = invItem;

    if (m_pInvItem->IconsTexture == nullptr)
    {
        m_pItemBrush = new CUIXBrush("ui\\ui_icon_equipment");   
    }
    else
    {
        m_pItemBrush = new CUIXBrush(m_pInvItem->IconsTexture);
    }
    //m_pItemBrush->SetSize(xr_vector2f().set(100.0f, 100.0f));
    m_pBackgroundBrush = new CUIXBrush(EUIXDrawType::image);*/
}

void CUIXInventoryItem::Draw()
{
    if (m_pItemBrush != nullptr)
    {
        m_pItemBrush->Render(GetRect(), m_renderTransform);
    }
}

xr_vector2f CUIXInventoryItem::GetDesiredSize()
{
    return inherited::GetDesiredSize();
}

