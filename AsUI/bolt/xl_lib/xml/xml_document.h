/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2007                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   xml_document.h 
*   Author      :   ¡ı÷«¥œ(liuzhicong@xunlei.com)
*   Create      :   2010ƒÍ11‘¬18»’
*   LastChange  :   
*   History     :	
*
*   Description :   XML Document
*
********************************************************************/ 
#ifndef __XML_DOCUMENT_H__
#define __XML_DOCUMENT_H__

#include <string>

#include "../tchar.h"
#include "./xml_node.h"

namespace xl
{
namespace xml
{

class xml_document : public xml_node
{
public:
    xml_document()
    {
        m_doc_org_encode = TSTRING("utf-8");
    }

public:
    const tchar * get_document_path()
    {
        return m_doc_path.c_str();
    }

    void set_document_path(const tchar* path)
    {
        m_doc_path = path;
    }

    const tchar* get_document_encode()
    {
        return m_doc_org_encode.c_str();
    }

    void set_document_encode(const tchar* encodename)
    {
        m_doc_org_encode = encodename;
    }

    virtual size_t size_of(void)
    {
        return 0;
    }
protected:
    xml_document(xml_document& )
    {

    }

    xml_document& operator= (xml_document& )
    {
        return *this;
    }

protected:
    tstring m_doc_path;
    tstring m_doc_org_encode;
};


}
}

#endif // __XML_DOCUMENT_H__