#pragma once
#include "../xrCore/FormatParsers/XML/xrXMLParser.h"

class CUIXBorder;
class CUIXTextBlock;
class CUIXml;
class CUIXWidget;
class CUIXImage;
class CUIXCanvas;

class CUIXHelper
{
public:
    CUIXHelper() = default;
    ~CUIXHelper() = default;

    static CUIXWidget* CreateWidget(CUIXml& xml, string_path path);
    
    static CUIXCanvas* CreateCanvas(CUIXml& xml, XML_NODE* canvasNode);
    
    static CUIXImage* CreateImage(CUIXml& xml, XML_NODE* imageNode);

    static CUIXTextBlock* CreateTextBlock(CUIXml& xml, XML_NODE* textNode);

    static CUIXBorder* CreateBorder(CUIXml& xml, XML_NODE* borderNode);
};
