#pragma once
#include "ui_x/UIXDefs.h"
#include "../../xrScripts/script_export_space.h"
#include "../../xrScripts/script_space_forward.h"

class CUIXSwitcherSlot;
class CUIXBorderSlot;
class CUIXCanvasSlot;
class CUIXSwitcher;
class CUIXTextBlock;
class CUIXCanvas;
class CUIXImage;
class CUIXWidget;
class CUIXBorder;

class CUIXElement
{
    static xr_atomic_u32 m_idGenerator;
    
    xr_string m_name;
    xr_rect_f m_absoluteRect;
public:
    // --- Constructors -----------------------------------------------
    CUIXElement(const shared_str& namePrefix);
    virtual ~CUIXElement();
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    virtual void Draw() = 0;
    virtual void Rebuild() = 0;
    // ----------------------------------------------------------------
    // --- Input ------------------------------------------------------
    virtual bool OnMouseMove(int dx, int dy) = 0;
    virtual bool OnKeyboardPressed(int key) = 0;
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    bool m_bDebugDrawPreviewFrame = true;
    virtual void RenderUIDebugNode();
    virtual void RenderUIDebugNodeChild() {}
    virtual void RenderUIDebugProperties();
    virtual void RenderUIDebugPreviewFrame(u32 m_bDebugDrawPreviewColor) {}
#endif
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    virtual void SetName(const shared_str& name) { m_name = name.c_str(); }
    const xr_string& GetName() const { return m_name; }

    void SetRect(const xr_rect_f& rect) { m_absoluteRect = rect; }
    void SetRect(const float x1, const float y1, const float x2, const float y2) { m_absoluteRect.set(x1, y1, x2, y2); }
    const xr_rect_f& GetRect() const { return m_absoluteRect; }
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};

class CUIXWidgetSlot : public CUIXElement
{
    using inherited = CUIXElement;
protected:
    CUIXWidget* m_pParent;
    CUIXWidget* m_pWidget;
    
public:
    // --- Constructors -----------------------------------------------
    CUIXWidgetSlot(CUIXWidget* parent) : inherited("widget_slot"), m_pParent(parent), m_pWidget(nullptr) {}
    CUIXWidgetSlot(const shared_str& namePrefix, CUIXWidget* parent) : inherited(namePrefix), m_pParent(parent), m_pWidget(nullptr) {}
    ~CUIXWidgetSlot() override;
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Draw() override;
    void Rebuild() override;
    // ----------------------------------------------------------------
    // --- Input ------------------------------------------------------
    bool OnMouseMove(int dx, int dy) override;
    bool OnKeyboardPressed(int key) override;
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugNodeChild() override;
    void RenderUIDebugPreviewFrame(u32 m_bDebugDrawPreviewColor) override;
#endif
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    virtual CUIXWidgetSlot*     ui_x_cast_widget_slot()     { return this; }
    virtual CUIXCanvasSlot*     ui_x_cast_canvas_slot()     { return nullptr; }
    virtual CUIXBorderSlot*     ui_x_cast_border_slot()     { return nullptr; }
    virtual CUIXSwitcherSlot*   ui_x_cast_switcher_slot()   { return nullptr; }
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    void            SetWidget       (CUIXWidget* widget);
    CUIXWidget*     GetWidget       () const { return m_pWidget; }
    
    CUIXWidget*     GetParent       () const { return m_pParent; }
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};

class CUIXWidget : public CUIXElement
{
    using inherited = CUIXElement;
    std::vector<luabind::functor<void>> m_mouseMoveCallbacks;
    std::vector<luabind::functor<void>> m_keyboardPressedCallbacks;
    std::vector<luabind::functor<void>> m_focusChangedCallbacks;
protected:
    CUIXWidgetSlot* m_pParentSlot;
    EUIXVisibility m_eVisibility;
    FUIXRenderTransform m_renderTransform;
public:
    // --- Constructors -----------------------------------------------
    CUIXWidget() : inherited("widget"), m_pParentSlot(nullptr), m_eVisibility(EUIXVisibility::NonHit){}
    CUIXWidget(const shared_str& namePrefix) : inherited(namePrefix), m_pParentSlot(nullptr),m_eVisibility(EUIXVisibility::NonHit){}
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
    void Rebuild() override {}
    virtual CUIXWidgetSlot* AttachChild(CUIXWidget* widget) { return nullptr; }
    // ----------------------------------------------------------------
    // --- Input ------------------------------------------------------
    bool OnMouseMove(int dx, int dy) override;
    bool OnKeyboardPressed(int key) override;
    virtual void OnFocusChanged(bool bIsFocused);
    // ----------------------------------------------------------------
    // --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif
    // ----------------------------------------------------------------
    // --- Casting ----------------------------------------------------
    virtual CUIXWidget*     ui_x_cast_widget()      { return this; }
    virtual CUIXBorder*     ui_x_cast_border()      { return nullptr; }
    virtual CUIXImage*      ui_x_cast_image()       { return nullptr; }
    virtual CUIXTextBlock*  ui_x_cast_text()        { return nullptr; }
    virtual CUIXCanvas*     ui_x_cast_canvas()      { return nullptr; } 
    virtual CUIXSwitcher*   ui_x_cast_switcher()    { return nullptr; } 
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
    void SetName(const shared_str& name) override;

    void SetParentSlot(CUIXWidgetSlot* parentSlot) { m_pParentSlot = parentSlot; }
    CUIXWidgetSlot* GetParentSlot() const { return m_pParentSlot; }
    
    void SetVisibility(EUIXVisibility visibility) { m_eVisibility = visibility; }
    EUIXVisibility GetVisibility() const { return m_eVisibility; }

    void SetRenderTransform(const FUIXRenderTransform& renderTransform) { m_renderTransform = renderTransform; }
    const FUIXRenderTransform& GetRenderTransform() const { return m_renderTransform; }
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
    void BindMouseMoveCallback(const luabind::functor<void>& func);
    void BindKeyboardPressedCallback(const luabind::functor<void>& func);
    void BindFocusChangedCallback(const luabind::functor<void>& func);
    DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};

