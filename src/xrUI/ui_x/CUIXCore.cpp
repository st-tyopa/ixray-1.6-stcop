#include "stdafx.h"
#include "CUIXCore.h"

#include "xrUIXmlParser.h"
#include "common/CUIXHelper.h"
#include "panels/CUIXCanvas.h"
#include "../xrEngine/XR_IOConsole.h"
#include "widgets/CUIXImage.h"
#include "widgets/CUIXTextBlock.h"

UI_API CUIXCore* g_uiXCore = nullptr;

UI_API CUIXCore& ui_x()	
{
    return *g_uiXCore;
}

CUIXCore::CUIXCore()
{
    //Device.seqFrame.Add(this, REG_PRIORITY_NORMAL);
    Device.seqDeviceReset.Add(this,REG_PRIORITY_NORMAL);
    Device.seqResolutionChanged.Add(this, REG_PRIORITY_NORMAL);
    
    m_eCurrentPointType = IUIRender::pttTL;
    Load();

#ifdef DEBUG_DRAW
    if (!Device.IsEditorMode())
    {
        CImGuiManager::Instance().Subscribe("UI_X_DEBUG", CImGuiManager::ERenderPriority::eMedium, xr_make_delegate(this, &CUIXCore::RenderDebugger));
    }
#endif
}

CUIXCore::~CUIXCore()
{
    //Device.seqFrame.Remove(this);
    Device.seqDeviceReset.Remove(this);
    Device.seqResolutionChanged.Remove(this);
    UnLoad();
}

void CUIXCore::Load()
{
    
    CUIXml uiXml;
    uiXml.Load(CONFIG_PATH, "ui_x", "layout.xml");
    if (CUIXWidget* widget = CUIXHelper::CreateWidget(uiXml, xr_strdup("canvas")))
    {
        m_pRoot = widget->ui_x_cast_canvas();
        //m_pRoot->SetRect(0.0f, 0.0f, float(Device.TargetWidth), float(Device.TargetHeight));
        
    }
    m_pFrustum2d.CreateFromRect(Frect().set(0.0f, 0.0f, float(Device.TargetWidth),float(Device.TargetHeight)));

#ifdef DEBUG_DRAW
    m_pDebugUIElement = nullptr;
#endif
    Device.seqRender.Add(this, REG_PRIORITY_LOW);
}

void CUIXCore::UnLoad()
{
    if (m_pRoot != nullptr)
    {
        xr_delete(m_pRoot);   
    }
#ifdef DEBUG_DRAW
    m_pDebugUIElement = nullptr;
#endif
    Device.seqRender.Remove(this);
}

void CUIXCore::OnRender()
{
    if (m_pRoot != nullptr)
    {
        m_pRoot->Draw();
    }
}

void CUIXCore::OnDeviceReset()
{
    UnLoad();
    Load();
}

void CUIXCore::OnScreenResolutionChanged()
{
    UnLoad();
    Load();
}

#ifdef DEBUG_DRAW
void CUIXCore::RenderDebugger()
{
    if (!Engine.External.EditorStates[static_cast<u8>(EditorUI::UI_X_General)])
    {
        return;
    }
    if (!ImGui::Begin("UI X Debugger", &Engine.External.EditorStates[static_cast<u8>(EditorUI::UI_X_General)]))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Reload UI"))
    {
		Console->Execute("ui_reload");
    }
    
    float color[4];
    color[0] = ((m_bDebugDrawPreviewColor >> 0) & 0xFF) / 255.0f; // R
    color[1] = ((m_bDebugDrawPreviewColor >> 8) & 0xFF) / 255.0f;  // G
    color[2] = ((m_bDebugDrawPreviewColor >> 16) & 0xFF) / 255.0f;  // B
    color[3] = ((m_bDebugDrawPreviewColor >> 24) & 0xFF) / 255.0f; // A
    if (ImGui::ColorEdit4("Preview Color", (float*)&color, ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB))
    {
        m_bDebugDrawPreviewColor = IM_COL32(u8(color[0] * 255.0f), u8(color[1] * 255.0f), u8(color[2] * 255.0f), u8(color[3] * 255.0f));
    }
    
    ImGui::Separator();
    if (ImGui::BeginChild("TreeRegion", ImVec2(0, ImGui::GetContentRegionAvail().y / 2), true)) {
        if (m_pRoot)
        {
            m_pRoot->RenderUIDebugNode();
        }
        ImGui::EndChild();
    }

    if (ImGui::BeginChild("InputRegion", ImVec2(0, 0), true)) {
        if (m_pDebugUIElement)
        {
            ImGui::Text("Selected: %s", m_pDebugUIElement->GetName().c_str());
            ImGui::Checkbox("Preview", &m_pDebugUIElement->m_bDebugDrawPreviewFrame);
            m_pDebugUIElement->RenderUIDebugProperties();
            m_pDebugUIElement->RenderUIDebugPreviewFrame(m_bDebugDrawPreviewColor);
        }
        else
        {
            ImGui::Text("No UI window selected.");
        }
        ImGui::EndChild();
    }
    
	ImGui::End();
}
#endif

void FUIXVertex2d::RotatePt(const xr_vector2f& pivot, const float cosA, const float sinA)
{
    xr_vector2f t = pt;
    t.sub(pivot);
    pt.x = t.x * cosA + t.y * sinA;
    pt.y = t.y * cosA - t.x * sinA;
    pt.add(pivot);
}

void CUIXFrustum2d::CreateFromRect(const xr_rect_f& rect)
{
    m_rect.set(float(rect.x1), float(rect.y1), float(rect.x2), float(rect.y2) );
    m_planes.resize(4);
    m_planes[0].build	(rect.lt, xr_vector2f().set(-1, 0));
    m_planes[1].build	(rect.lt, xr_vector2f().set( 0,-1));
    m_planes[2].build	(rect.rb, xr_vector2f().set(+1, 0));
    m_planes[3].build	(rect.rb, xr_vector2f().set( 0,+1));
}

CUIXPoly2dSafe* CUIXFrustum2d::ClipPoly(CUIXPoly2dSafe& S, CUIXPoly2dSafe& D) const
{
    bool use_full_test = false;
    for (u32 j=0; j<S.size(); j++)
    {
        if( !m_rect.in(S[j].pt) ) {
            use_full_test = true;
            break;
        }
    }

    CUIXPoly2dSafe* src = &D;
    CUIXPoly2dSafe* dest	= &S;
    if(!use_full_test)
    {
        return dest;
    }

    for (u32 i=0; i<m_planes.size(); i++)
    {
        // cache plane and swap lists
        const xr_plane2f &plane	= m_planes[i];
        xr_swap(src,dest);
        dest->clear();

        // classify all points relative to plane #i
        float cls[UI_X_FRUSTUM_SAFE];
        for (u32 j=0; j<src->size(); j++)
        {
            cls[j] = plane.classify((*src)[j].pt);
        }

        // clip everything to this plane
        cls[src->size()] = cls[0]	;
        src->push_back((*src)[0])	;
        xr_vector2f dir_pt,dir_uv;
        float denum,t;
        for (u32 j=0; j<src->size()-1; j++)	{
            if ( (*src)[j].pt.similar( (*src)[j+1].pt,EPS_S ) )
            {
                continue;
            }
            if ( negative( cls[j] ) )
            {
                dest->push_back( (*src)[j] );
                if (positive(cls[j+1]))	{
                    // segment intersects plane
                    dir_pt.sub((*src)[j+1].pt,(*src)[j].pt);
                    dir_uv.sub((*src)[j+1].uv,(*src)[j].uv);
                    denum = plane.n.dotproduct(dir_pt);
                    if (denum != 0) {
                        t = -cls[j] / denum	; //VERIFY(t<=1.f && t>=0);
                        dest->last().pt.mad	((*src)[j].pt,dir_pt,t);
                        dest->last().uv.mad	((*src)[j].uv,dir_uv,t);
                        dest->inc();
                    }
                }
            }
            else
            {
                // J - outside
                if ( negative( cls[j+1] ) )	{
                    // J+1  - inside
                    // segment intersects plane
                    dir_pt.sub((*src)[j+1].pt,(*src)[j].pt);
                    dir_uv.sub((*src)[j+1].uv,(*src)[j].uv);
                    denum = plane.n.dotproduct(dir_pt);
                    if (denum != 0)	{
                        t = -cls[j]/denum	; //VERIFY(t<=1.f && t>=0);
                        dest->last().pt.mad	((*src)[j].pt,dir_pt,t);
                        dest->last().uv.mad	((*src)[j].uv,dir_uv,t);
                        dest->inc();
                    }
                }
            }
        }

        // here we end up with complete polygon in 'dest' which is inside plane #i
        if (dest->size() < 3)
        {
            return nullptr;
        }
    }
    return dest;
}
