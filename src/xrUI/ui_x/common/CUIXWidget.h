#pragma once
#include "ui_x/UIXDefs.h"

class CUIXTextBlock;
class CUIXCanvas;
class CUIXImage;
class CUIXWidget;

class CUIXElement
{
    static xr_atomic_u32 m_idGenerator;
    
    xr_string m_name;
    xr_rect_f m_absoluteRect;
public:
    CUIXElement(const shared_str& namePrefix);
    virtual ~CUIXElement();
    
    virtual void Draw() = 0;
    virtual void Rebuild() = 0;

#ifdef DEBUG_DRAW
    bool m_bDebugDrawPreviewFrame = true;
    virtual void RenderUIDebugNode();
    virtual void RenderUIDebugNodeChild() {}
    virtual void RenderUIDebugProperties();
    virtual void RenderUIDebugPreviewFrame(u32 m_bDebugDrawPreviewColor) {}
#endif

    void SetName(const shared_str& name) { m_name = name.c_str(); }
    const xr_string& GetName() const { return m_name; }

    void SetRect(const xr_rect_f& rect) { m_absoluteRect = rect; }
    void SetRect(const float x1, const float y1, const float x2, const float y2) { m_absoluteRect.set(x1, y1, x2, y2); }
    const xr_rect_f& GetRect() const { return m_absoluteRect; }
};

class CUIXWidgetSlot : public CUIXElement
{
    using inherited = CUIXElement;
protected:
    CUIXWidget* m_pParent;
    CUIXWidget* m_pWidget;
    
public:
    CUIXWidgetSlot(CUIXWidget* parent) : inherited("widget_slot"), m_pParent(parent), m_pWidget(nullptr) {}
    CUIXWidgetSlot(const shared_str& namePrefix, CUIXWidget* parent) : inherited(namePrefix), m_pParent(parent), m_pWidget(nullptr) {}
    ~CUIXWidgetSlot() override;

    void Draw() override;
    void Rebuild() override;

#ifdef DEBUG_DRAW
    void RenderUIDebugNodeChild() override;
    void RenderUIDebugPreviewFrame(u32 m_bDebugDrawPreviewColor) override;
#endif

    void            SetWidget       (CUIXWidget* widget) { m_pWidget = widget; }
    CUIXWidget*     GetWidget       () const { return m_pWidget; }
    
    CUIXWidget*     GetParent       () const { return m_pParent; }
};

class CUIXWidget : public CUIXElement
{
    using inherited = CUIXElement;
protected:
    EUIXVisibility m_eVisibility;
    FUIXRenderTransform m_renderTransform;
public:
    CUIXWidget() : inherited("widget"), m_eVisibility(EUIXVisibility::Visible){}
    CUIXWidget(const shared_str& namePrefix) : inherited(namePrefix), m_eVisibility(EUIXVisibility::Visible){}

    //virtual void Draw() = 0;
    void Rebuild() override {}

#ifdef DEBUG_DRAW
    void RenderUIDebugProperties() override;
#endif
    
    // --- Casting ---------------------------------------------------
    virtual CUIXWidget*     ui_x_cast_widget()  { return this; }
    virtual CUIXImage*      ui_x_cast_image()   { return nullptr; }
    virtual CUIXTextBlock*  ui_x_cast_text()    { return nullptr; }
    virtual CUIXCanvas*     ui_x_cast_canvas()  { return nullptr; } 
    // ---------------------------------------------------------------
    
    /** getters and setters */
    void SetVisibility(EUIXVisibility visibility) { m_eVisibility = visibility; }
    EUIXVisibility GetVisibility() const { return m_eVisibility; }

    void SetRenderTransform(const FUIXRenderTransform& renderTransform) { m_renderTransform = renderTransform; }
    const FUIXRenderTransform& GetRenderTransform() const { return m_renderTransform; }
};
