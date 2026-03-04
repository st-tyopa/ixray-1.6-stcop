#pragma once

class CUIXTextBlock;
class CUIXFontManager;
class CUIXElement;
class CUIXCanvas;
class CUIXCursor;

#include "UIXDefs.h"

class UI_API CUIXCore :
	public pureRender,
	public pureDeviceReset,
	public pureScreenResolutionChanged
{
	CUIXFrustum2d m_pFrustum2d;
	
	CUIXCanvas* m_pRoot;

	//CUIXTextBlock* m_pTextBlock;
public:
    IUIRender::ePointType m_eCurrentPointType;

    CUIXCore();
    virtual ~CUIXCore();

	void Load();
	void UnLoad();
	
	void OnRender() override;
	void OnDeviceReset() override;
	void OnScreenResolutionChanged() override;

	const CUIXFrustum2d& frustum()	const { return m_pFrustum2d; }

#ifdef DEBUG_DRAW
	CUIXElement* m_pDebugUIElement;
	u32 m_bDebugDrawPreviewColor = 0xFFFFFFFF;
	void RenderDebugger();
#endif
};

extern UI_API CUIXCore& ui_x();
extern UI_API CUIXCore* g_uiXCore;