#pragma once

#include "../../xrScripts/script_export_space.h"

class CUIXImage;
class CUIXWidget;
class CUIXSwitcher;
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
	CUIXCanvas* m_pRoot;
	CUIXWidget* m_pCursor;
	CUIXSwitcher* m_pDialogHolder;
	CUIXWidget* m_pFocusedWidget;
	xr_hash_map<shared_str, CUIXWidget*> m_elementsCache;

	bool m_bIsReceiveMouseInput = false;
	bool m_bIsReceiveKeyBoardInput = false;	
	
public:
    // --- Constructors -----------------------------------------------
    CUIXCore();
    virtual ~CUIXCore();
    // ----------------------------------------------------------------
    // --- Control ----------------------------------------------------
	void Load();
	void UnLoad();
    // ----------------------------------------------------------------
    // --- Pure events ------------------------------------------------
	void OnRender() override;
	void OnDeviceReset() override;
	void OnScreenResolutionChanged() override;
    // ----------------------------------------------------------------
    // --- Widget Cache -----------------------------------------------
	void RegisterWidgetId(const shared_str& id, CUIXWidget* pElement);
	void UnregisterWidgetId(const shared_str& id);
	CUIXWidget* FindWidget(const shared_str& id);
	void ClearCache() { m_elementsCache.clear(); }
    // ----------------------------------------------------------------
	// --- Dialog Control ---------------------------------------------
	CUIXWidget* AddDialog(LPCSTR name, LPCSTR xmlPath);
	CUIXWidget* ShowDialog(LPCSTR name);
	CUIXWidget* ShowDialog(CUIXWidget* pDialog);
	void HideCurrentDialog();
    // ----------------------------------------------------------------
    // --- Input ------------------------------------------------------
	bool IsReceiveMouseInput() const { return m_bIsReceiveMouseInput; }
	bool IsReceiveKeyBoardInput() const { return m_bIsReceiveKeyBoardInput; }
	void UpdateInputMode(bool bCaptureMouse = false, bool bCaptureKeyboard = false);
	bool OnMouseMove(int dx, int dy);
	bool OnMouseHold(int key);
	bool OnMousePress(int key);
	bool OnMouseRelease(int key);
	bool OnMouseWheel(int direction);
	bool OnKeyboardPress(int key);
	bool OnKeyboardRelease(int key);
	bool OnKeyboardHold(int key);
    // ----------------------------------------------------------------
    // --- Getters And Setters ----------------------------------------
	xr_vector2f GetCursorPosition() const;
	
	void SetFocusedWidget(CUIXWidget* pWidget);
	CUIXWidget* GetFocusedWidget() const { return m_pFocusedWidget; }
    // ----------------------------------------------------------------
	// --- Debug info -------------------------------------------------
#ifdef DEBUG_DRAW
	CUIXElement* m_pDebugUIElement;
	u32 m_bDebugDrawPreviewColor = 0xFFFFFFFF;
	void RenderDebugger();
#endif
    // ----------------------------------------------------------------
    // --- Script Register --------------------------------------------
	DECLARE_SCRIPT_REGISTER_FUNCTION
    // ----------------------------------------------------------------
};

extern UI_API CUIXCore& ui_x();
extern UI_API CUIXCore* g_uiXCore;