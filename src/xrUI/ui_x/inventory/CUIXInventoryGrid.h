#pragma once
#include "ui_x/panels/CUIXCanvas.h"

class CUIXBrush;
class CInventoryItem;

class CUIXInventoryGrid : public CUIXCanvas
{
    using inherited = CUIXCanvas;
    CUIXBrush* m_pGridBrush = nullptr;
    xr_vector2i m_gridSize;
    xr_vector2f m_cellSize; // cell size in pixels
    //u32 m_gridWidth = 1;    // cells count in 1 grid row
    //u32 m_availableCells = 1;
public:
    // --- Constructors -----------------------------------------------
    CUIXInventoryGrid();
    ~CUIXInventoryGrid() override = default;
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    CUIXInventoryGrid* ui_x_cast_inventory_grid() override { return this; }
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Draw() override;
    void Rebuild() override;
    bool PushItem(CInventoryItem* pInvItem);
    bool PopItem(CInventoryItem* pInvItem);
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    void SetGridBrush(CUIXBrush* pGridBrush) { m_pGridBrush = pGridBrush; }
    CUIXBrush* GetGridBrush() const { return m_pGridBrush; }

    void SetGridSize(xr_vector2i gridSize) { m_gridSize = gridSize; }
    xr_vector2i GetGridSize() const { return m_gridSize; }

    void SetCellSize(xr_vector2f cellSize) { m_cellSize = cellSize; }
    xr_vector2f GetCellSize() const { return m_cellSize; }
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
};
