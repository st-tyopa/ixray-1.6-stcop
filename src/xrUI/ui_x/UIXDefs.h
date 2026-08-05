#pragma once

#include "../Include/xrRender/FactoryPtr.h"
#include "../Include/xrRender/UIRender.h"
#include "../Include/xrRender/UIShader.h"

#define UI_X    // comment this if you want to use legacy UI

using ui_x_shader = FactoryPtr<IUIShader>;
using xr_vector2f = _vector2<float>;
using xr_vector2i = _vector2<int>;
using xr_plane2f = _plane2<float>;
using xr_rect_f = _rect<float>;

enum class EUIXVisibility : u8
{
    Visible,
    Hidden,
    Collapsed,
    NonHit,
    NonHitWithChild
};

enum class EUIXAlignVertical : u8
{
    Top,
    Center,
    Buttom,
    Fill
};

enum class EUIXAlignHorizontal : u8
{
    Left,
    Center,
    Right,
    Fill
};

struct UI_API FUIXVertex2d
{
    xr_vector2f	pt;
    xr_vector2f	uv;
    FUIXVertex2d () {}
    FUIXVertex2d (float pX, float pY, float tU, float tV) { pt.set(pX,pY); uv.set(tU,tV); }
    void Set (float pt_x, float pt_y, float uv_x, float uv_y) { pt.set(pt_x,pt_y); uv.set(uv_x,uv_y); }
    void Set (const xr_vector2f& _pt, const xr_vector2f& _uv) { pt.set(_pt); uv.set(_uv); }
    void RotatePt (const xr_vector2f& pivot, const float cosA, const float sinA);
};

struct UI_API FUIXRenderTransform
{
    float angle;
    xr_vector2f	pivot;
    xr_vector2f	scale;
    FUIXRenderTransform ()
    {
        angle = 0;
        pivot.set(0.5f, 0.5f);
        scale.set(1.0f, 1.0f);
    }
};

#define UI_X_FRUSTUM_MAXPLANES 12
#define UI_X_FRUSTUM_SAFE (UI_X_FRUSTUM_MAXPLANES * 4)
using CUIXPoly2dSafe = svector<FUIXVertex2d, UI_X_FRUSTUM_SAFE>;
using CUIXPoly2dMax = svector<xr_plane2f, UI_X_FRUSTUM_MAXPLANES>;


class UI_API CUIXFrustum2d
{
    CUIXPoly2dMax m_planes;
    xr_rect_f m_rect;
public:
    void CreateFromRect (const xr_rect_f& rect);
    CUIXPoly2dSafe* ClipPoly (CUIXPoly2dSafe& s, CUIXPoly2dSafe& d) const;
};

