#include "stdafx.h"
#include "CUIXHelper.h"

#include "CUIXBrush.h"
#include "xrUIXmlParser.h"
#include "../xrEngine/string_table.h"
#include "ui_x/panels/CUIXCanvas.h"
#include "ui_x/widgets/CUIXBorder.h"
#include "ui_x/widgets/CUIXImage.h"
#include "ui_x/widgets/CUIXTextBlock.h"

CUIXWidget* CUIXHelper::CreateWidget(CUIXml& xml, string_path path)
{
    if (XML_NODE* node = xml.NavigateToNode(path, 0))
    {
        xml.SetLocalRoot(node);
        const xr_string widgetId = xml.ReadAttrib(node, "id", "_");
        const xr_string nodeName = node->Value();
        CUIXWidget* widget = nullptr;
        if (nodeName == "canvas")
        {
            widget = CreateCanvas(xml, node)->ui_x_cast_widget();
        }
        else if (nodeName == "image")
        {
            widget = CreateImage(xml, node)->ui_x_cast_widget();
        }
        else if (nodeName == "text")
        {
            widget =  CreateTextBlock(xml, node)->ui_x_cast_widget();
        }
        else if (nodeName == "border")
        {
            widget =  CreateBorder(xml, node)->ui_x_cast_widget();
        }
        
        if (widget && widgetId != "_")
        {
            widget->SetName(widgetId.c_str());
        }
        return widget;
    }
    return nullptr;
}

CUIXCanvas* CUIXHelper::CreateCanvas(CUIXml& xml, XML_NODE* canvasNode)
{
    //
    CUIXCanvas* canvas = new CUIXCanvas();
    xr_vector2f size;
    size.x = xml.ReadAttribFlt(canvasNode, "w", float(Device.TargetWidth));
    size.y = xml.ReadAttribFlt(canvasNode, "h", float(Device.TargetHeight));
    canvas->SetRect(0.0f, 0.0f, size.x, size.y);

    const int slotCount = xml.GetNodesNum(canvasNode, "canvas_slot");
    //string_path slotPath;
    //xr_strconcat(slotPath, path, ":canvas_slot");
    for (int i = 0; i < slotCount; i++)
    {
        // read child widget
        XML_NODE* slotNode = xml.NavigateToNode("canvas_slot", i);
        if (slotNode->FirstChild() == nullptr)
        {
            continue;
        }
        xml.SetLocalRoot(slotNode);
        //string_path childPath;
        //xr_strconcat(childPath, path, ":canvas_slot:",node->FirstChild()->Value());
        CUIXWidget* widget = CreateWidget(xml, xr_strdup(slotNode->FirstChild()->Value()));
        if (widget == nullptr)
        {
            continue;
        }
        // setup canvas slot 
        xml.SetLocalRoot(slotNode);
        CUIXCanvasSlot* slot = canvas->AttachChild(widget);
        
        xr_vector2f slotSize;
        slotSize.x = xml.ReadAttribFlt(xml.GetLocalRoot(), "w", 64.0f);
        slotSize.y = xml.ReadAttribFlt(xml.GetLocalRoot(), "h", 64.0f);
        slot->SetSize(slotSize, false);
        
        xr_vector2f slotPosition;
        slotPosition.x = xml.ReadAttribFlt(xml.GetLocalRoot(), "x", 0.0f);
        slotPosition.y = xml.ReadAttribFlt(xml.GetLocalRoot(), "y", 0.0f);
        slot->SetPosition(slotPosition, false);

        xr_vector2f slotAlignment;
        slotAlignment.x = xml.ReadAttribFlt(xml.GetLocalRoot(), "ax", 0.0f);
        slotAlignment.y = xml.ReadAttribFlt(xml.GetLocalRoot(), "ay", 0.0f);
        slot->SetAlignment(slotAlignment, false);

        const xr_string anchor = xml.ReadAttrib(xml.GetLocalRoot(), "anchor", "lt");
        
        if (anchor == "lt")      { slot->SetAnchor(EUIXAnchor::ltAnchor, false); }
        else if (anchor == "lc") { slot->SetAnchor(EUIXAnchor::lcAnchor, false); }
        else if (anchor == "lb") { slot->SetAnchor(EUIXAnchor::lbAnchor, false); }
        else if (anchor == "ct") { slot->SetAnchor(EUIXAnchor::ctAnchor, false); }
        else if (anchor == "cc") { slot->SetAnchor(EUIXAnchor::ccAnchor, false); }
        else if (anchor == "cb") { slot->SetAnchor(EUIXAnchor::cbAnchor, false); }
        else if (anchor == "rt") { slot->SetAnchor(EUIXAnchor::rtAnchor, false); }
        else if (anchor == "rc") { slot->SetAnchor(EUIXAnchor::rcAnchor, false); }
        else if (anchor == "rb") { slot->SetAnchor(EUIXAnchor::rbAnchor, false); }
        else if (anchor == "ft") { slot->SetAnchor(EUIXAnchor::ftAnchor, false); }
        else if (anchor == "fc") { slot->SetAnchor(EUIXAnchor::fcAnchor, false); }
        else if (anchor == "fb") { slot->SetAnchor(EUIXAnchor::fbAnchor, false); }
        else if (anchor == "lf") { slot->SetAnchor(EUIXAnchor::lfAnchor, false); }
        else if (anchor == "cf") { slot->SetAnchor(EUIXAnchor::cfAnchor, false); }
        else if (anchor == "rf") { slot->SetAnchor(EUIXAnchor::rfAnchor, false); }
        else if (anchor == "ff") { slot->SetAnchor(EUIXAnchor::ffAnchor, false); }

        slot->Rebuild();

        xml.SetLocalRoot(canvasNode);
    }
    
    return canvas;
}

CUIXImage* CUIXHelper::CreateImage(CUIXml& xml, XML_NODE* imageNode)
{
    CUIXImage* image;// = nullptr;
    if (XML_NODE* node = xml.NavigateToNode("brush", 0))
    {
        const shared_str textureName = xml.ReadAttrib(node, "tx", UI_X_DEFAULT_TEXTURE_NAME);
        const shared_str shaderName = xml.ReadAttrib(node, "sh", UI_X_DEFAULT_SHADER_NAME);
        CUIXBrush* brush = new CUIXBrush(textureName, shaderName);

        xr_string tint = xml.ReadAttrib(node, "tint", "0xFFFFFFFF");
        if (tint == "0xFFFFFFFF")
        {
            const u8 tint_a = (u8)xml.ReadAttribInt(node, "a", 255);
            const u8 tint_r = (u8)xml.ReadAttribInt(node, "r", 255);
            const u8 tint_g = (u8)xml.ReadAttribInt(node, "g", 255);
            const u8 tint_b = (u8)xml.ReadAttribInt(node, "b", 255);
            brush->SetTint(tint_a, tint_r, tint_g, tint_b);   
        }
        else
        {
            brush->SetTint((u32)strtoul(tint.c_str(), nullptr, 16));
        }
        
        image = new CUIXImage(brush);
    }
    else
    {
        image = new CUIXImage(new CUIXBrush());
    }
    
    return image;
}

CUIXTextBlock* CUIXHelper::CreateTextBlock(CUIXml& xml, XML_NODE* textNode)
{
    CUIXTextBlock* textBlock = new CUIXTextBlock();
    if (XML_NODE* node = xml.NavigateToNode("brush", 0))
    {
        xr_string tint = xml.ReadAttrib(node, "tint", "0xFFFFFFFF");
        if (tint == "0xFFFFFFFF")
        {
            const u8 tint_a = (u8)xml.ReadAttribInt(node, "a", 255);
            const u8 tint_r = (u8)xml.ReadAttribInt(node, "r", 255);
            const u8 tint_g = (u8)xml.ReadAttribInt(node, "g", 255);
            const u8 tint_b = (u8)xml.ReadAttribInt(node, "b", 255);
            textBlock->SetColor(tint_a, tint_r, tint_g, tint_b);   
        }
        else
        {
            textBlock->SetColor((u32)strtoul(tint.c_str(), nullptr, 16));
        }
    }
    if (XML_NODE* node = xml.NavigateToNode("content", 0))
    {        
        xr_string fontName = xml.ReadAttrib(node, "fnt", "ui_font_letterica16_russian");
        textBlock->SetFont(g_FontManager->GetFont(fontName.c_str(), 0));

        xr_string text = xml.Read(node, "");
        textBlock->SetText(text.c_str());
    }
    return textBlock;
}

CUIXBorder* CUIXHelper::CreateBorder(CUIXml& xml, XML_NODE* borderNode)
{
    CUIXBorder* border;
    if (XML_NODE* node = xml.NavigateToNode("brush", 0))
    {
        const shared_str textureName = xml.ReadAttrib(node, "tx", UI_X_DEFAULT_TEXTURE_NAME);
        const shared_str shaderName = xml.ReadAttrib(node, "sh", UI_X_DEFAULT_SHADER_NAME);
        CUIXBrush* brush = new CUIXBrush(textureName, shaderName);

        xr_string tint = xml.ReadAttrib(node, "tint", "0xFFFFFFFF");
        if (tint == "0xFFFFFFFF")
        {
            const u8 tint_a = (u8)xml.ReadAttribInt(node, "a", 255);
            const u8 tint_r = (u8)xml.ReadAttribInt(node, "r", 255);
            const u8 tint_g = (u8)xml.ReadAttribInt(node, "g", 255);
            const u8 tint_b = (u8)xml.ReadAttribInt(node, "b", 255);
            brush->SetTint(tint_a, tint_r, tint_g, tint_b);   
        }
        else
        {
            brush->SetTint((u32)strtoul(tint.c_str(), nullptr, 16));
        }
        
        border = new CUIXBorder(brush);
    }
    else
    {
        border = new CUIXBorder(new CUIXBrush());
    }

    const int slotCount = xml.GetNodesNum(borderNode, "border_slot");
    for (int i = 0; i < slotCount; i++)
    {
        // read child widget
        XML_NODE* slotNode = xml.NavigateToNode("border_slot", i);
        if (slotNode->FirstChild() == nullptr)
        {
            continue;
        }
        xml.SetLocalRoot(slotNode);
        CUIXWidget* widget = CreateWidget(xml, xr_strdup(slotNode->FirstChild()->Value()));
        if (widget == nullptr)
        {
            continue;
        }
        // setup slot 
        xml.SetLocalRoot(slotNode);
        CUIXBorderSlot* slot = border->AttachChild(widget);
        
        float padding = xml.ReadAttribFlt(xml.GetLocalRoot(), "p", 2.0f);
        slot->SetPadding(padding, false);
        
        xr_rect_f paddingRect = slot->GetPadding();
        paddingRect.x1 = xml.ReadAttribFlt(xml.GetLocalRoot(), "pl", paddingRect.x1);
        paddingRect.y1 = xml.ReadAttribFlt(xml.GetLocalRoot(), "pt", paddingRect.y1);
        paddingRect.x2 = xml.ReadAttribFlt(xml.GetLocalRoot(), "pr", paddingRect.x2);
        paddingRect.y2 = xml.ReadAttribFlt(xml.GetLocalRoot(), "pb", paddingRect.y2);
        slot->SetPadding(paddingRect, false);

        slot->Rebuild();

        xml.SetLocalRoot(borderNode);
    }
    
    
    return border;
}
