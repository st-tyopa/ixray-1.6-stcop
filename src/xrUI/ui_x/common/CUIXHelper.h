#pragma once
#include "../xrCore/FormatParsers/XML/xrXMLParser.h"

class CUIXSwitcher;
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

    static CUIXWidget* CreateWidget(CUIXml& xml, string_path path, const shared_str& prefix);
    
    static CUIXCanvas* CreateCanvas(CUIXml& xml, XML_NODE* canvasNode, const shared_str& prefix);
    
    static CUIXImage* CreateImage(CUIXml& xml, XML_NODE* imageNode, const shared_str& prefix);

    static CUIXTextBlock* CreateTextBlock(CUIXml& xml, XML_NODE* textNode, const shared_str& prefix);

    static CUIXBorder* CreateBorder(CUIXml& xml, XML_NODE* borderNode, const shared_str& prefix);

    static CUIXSwitcher* CreateSwitcher(CUIXml& xml, XML_NODE* switcherNode, const shared_str& prefix);
};
