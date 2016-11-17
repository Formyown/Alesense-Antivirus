/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, LTD. 2009                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XMLDOMParser.h
*   Author      :   刘智聪(liuzhicong@xunlei.com)
*   Create      :   8:1:2009  
*   LastChange  :   2009/01/08
*   History     :	
*								
*   Description :   
*
********************************************************************/


/********************************************************************
*
* =-----------------------------------------------------------------=
* =                          ____          _________                =
* =                         / _  \         \___  __/                =
* =                        / /_/ /     ____   / /                   =
* =                       /  _   \ ● / _  \ / /                    =
* =                      /  /_/  // // / / // /                     =
* =                      \______//_//_/ /_//_/                      =
* =                                                                 =
* =             Copyright (c) BIN Technology studio,2004 - 2008     =
* =                           LET'Z BT                              =
* =-----------------------------------------------------------------=
* 
*   FileName    :   DOMParser (<bint/xml/DOMParser.hpp>)
*   Author      :   waterflier (waterflier@gmail.com)
*   Create      :   2006-9-8 22:28
*   LastChange  :   2006-9-12 22:19
*   History     :	1) 基本完成
*                      waterflier (2006-12-18 19:36) 
*								
*   Description :   class "DOMParser" 是一个简单的XML DOM解析器 
*
********************************************************************/  

#ifndef _XUNLEI_DOM_PARSER_H_
#define _XUNLEI_DOM_PARSER_H_

#include <string>

#include "xl_lib/text/transcode.h"

#include "./expat_parser.h"
#include "./xml_element.h"
#include "../tchar.h"

namespace xl
{

namespace xml
{



template<typename T>    
class basic_xml_dom_parser : public  expat_parser<basic_xml_dom_parser<T> >
{
public:
    basic_xml_dom_parser() : m_elements(16),m_parseDepth(-1)
	{
		m_rootElement = NULL;
        m_currentCData = NULL;
	}
	
	virtual ~basic_xml_dom_parser()
	{
		if(m_rootElement)
			m_rootElement->release();
		if(m_currentCData)
			m_currentCData->release();
	}	
	
	
public:
    basic_xml_element<T>*  get_result()
	{
		if(m_rootElement)
			m_rootElement->add_ref();

		return m_rootElement;
	}

public: 
    //NOTICE! there function NOT BE CALLED BY USER!
	void OnPostCreate()
	{
		// Enable all the event routines we want
		this->enable_element_handler();
		this->enable_character_data_handler();
		this->enable_unknown_encoding_handler();
		m_parseDepth = -1;
		m_isFinish = false;
	}

    void ManualSetParseError()
	{
		this->stop_parser();
	}

	inline void OnStartElement (const XML_Char *pszName, const XML_Char **papszAttrs);
	// End element handler
	void OnEndElement (const XML_Char *)
	{
		//save CharacterData on buffer
		if(m_parseDepth == 0)
		{
			m_isFinish = true;
		}
		else
		{
			if(m_elements[m_parseDepth-1])
				m_elements[m_parseDepth-1]->add_child_element(m_elements[m_parseDepth]);
		}
		
		if(m_currentCData)
		{
			m_elements[m_parseDepth]->append_cdata(m_currentCData);
			m_currentCData->release();
			m_currentCData = NULL;
		}
		
		m_elements[m_parseDepth]->release();
		m_elements[m_parseDepth] = NULL;
		m_parseDepth--;
		return;
	}
	// Character data handler
	inline void OnCharacterData (const XML_Char *pszData, int nLength);
    static int XMLConvertGB(void *, const char *s)
    {
        //把gb2312转换到ucs2编码,返回对应的ucs2编码值
        //,然后expat会把编码转换到utf-8编码再传给parsedata
        std::wstring wstr;
        xl::text::transcode::GBK_to_Unicode(s,2,wstr);
        return wstr[0];
    }

    bool OnUnknownEncoding (const XML_Char *pszName, XML_Encoding *pInfo)
	{
        int i;
        if( !pszName ||  strcmp(pszName,"gb2312") )
            return XML_STATUS_ERROR;

        for(i=0;i<128;i++)
            pInfo->map[i] = i;
        for(;i<256;i++)
            pInfo->map[i] = -2;
        pInfo->convert = XMLConvertGB;
        pInfo->release = NULL;
        return XML_STATUS_OK;
	}

public:
    bool is_finish()
    {
        return m_isFinish;
    }

	bool load_from_file(const tchar *pszPath)
	{
		char buffer[1024 * 16];
		int readlen;
		bool fSuccess = true;
		FILE* fp;
		#ifdef __LINUX__
			fp = fopen(pszPath,("rb"));
		#else
			fp = _tfopen(pszPath,TSTRING("rb"));
		#endif

		if(fp == NULL)
		{   
			return false;
		}

		this->create();
		while (!feof (fp) && fSuccess)
		{
			readlen = (int)fread(buffer,(int)sizeof(char),1024 * 16,fp);
			fSuccess = this->parse(buffer,readlen,0);
		}

        if(fSuccess && m_rootElement)
        {
            xml_document* pdoc = new xml_document();
            pdoc->set_document_path(pszPath);
            m_rootElement->set_document(pdoc);
            pdoc->release();
        }

		fclose(fp);
		return fSuccess;
	}

protected:


protected:
    std::vector<basic_xml_element<T>*> m_elements;
	basic_xml_element<T>* m_rootElement;
	basic_xml_cdata<T>* m_currentCData;

    long m_parseDepth;
    bool m_isFinish;
     
protected:
	basic_xml_dom_parser(const basic_xml_dom_parser& rhs)
	{
	}
	
	basic_xml_dom_parser& operator= (const basic_xml_dom_parser& rhs)
	{
		return *this;
	}
	
};	

template<>
inline void basic_xml_dom_parser<char>::OnStartElement (const XML_Char *pszName, const XML_Char **papszAttrs)
{
	m_parseDepth++;
	if(m_parseDepth >= (long)m_elements.size())
	{
		m_elements.push_back(NULL);
	}
	int attrpos = 0;

	if(m_parseDepth == 0)
	{
		m_rootElement = new basic_xml_element<char>(pszName);
		m_elements[m_parseDepth] = m_rootElement;
        m_rootElement->add_ref();

		while(papszAttrs[attrpos] != NULL)
		{
			m_elements[m_parseDepth]->set_attribute(papszAttrs[attrpos],papszAttrs[attrpos+1]);
			attrpos+=2;
		}
	}
	else
	{
		assert(m_elements[m_parseDepth-1]);
		if(m_currentCData)
		{
			m_elements[m_parseDepth-1]->append_cdata(m_currentCData);
			m_currentCData->release();
			m_currentCData = NULL;
		}

		m_elements[m_parseDepth] = new basic_xml_element<char>(pszName,m_elements[m_parseDepth-1]);
		while(papszAttrs[attrpos] != NULL)
		{
			m_elements[m_parseDepth]->set_attribute(papszAttrs[attrpos],papszAttrs[attrpos+1]);
			attrpos+=2;
		}
	}

	return;
}

template<>
inline void basic_xml_dom_parser<char>::OnCharacterData (const XML_Char *pszData, int nLength)
{
	if(m_currentCData == NULL)
	{
		m_currentCData = new basic_xml_cdata<char>();
	}

	m_currentCData->append_value(pszData,nLength);
	return;
}

template<>
inline  void basic_xml_dom_parser<wchar_t>::OnStartElement (const XML_Char *pszName, const XML_Char **papszAttrs)
{
	m_parseDepth++;
	if(m_parseDepth >= (long)m_elements.size())
	{
		m_elements.push_back(NULL);
	}

	int attrpos = 0;

	if(m_parseDepth == 0)
	{
        std::wstring utf16Name;
        xl::text::transcode::UTF8_to_Unicode(pszName,(xl::uint32)strlen(pszName),utf16Name);
		m_rootElement = new basic_xml_element<wchar_t>(utf16Name.c_str());
		m_rootElement->add_ref();
		m_elements[m_parseDepth] = m_rootElement;

		while(papszAttrs[attrpos] != NULL)
		{
			std::wstring utf16AttrName,utf16AttrValue;
            xl::text::transcode::UTF8_to_Unicode(papszAttrs[attrpos],(xl::uint32)strlen(papszAttrs[attrpos]),utf16AttrName);
			xl::text::transcode::UTF8_to_Unicode(papszAttrs[attrpos+1],(xl::uint32)strlen(papszAttrs[attrpos+1]),utf16AttrValue);
			m_elements[m_parseDepth]->set_attribute(utf16AttrName.c_str(),utf16AttrValue.c_str());
			attrpos+=2;
		}
	}
	else
	{
		assert(m_elements[m_parseDepth-1]);
		if(m_currentCData)
		{
			m_elements[m_parseDepth-1]->append_cdata(m_currentCData);
			m_currentCData->release();
			m_currentCData = NULL;
		}

        std::wstring utf16Name;
        xl::text::transcode::UTF8_to_Unicode(pszName,(xl::uint32)strlen(pszName),utf16Name);
		m_elements[m_parseDepth] = new basic_xml_element<wchar_t>(utf16Name.c_str(),m_elements[m_parseDepth-1]);
		while(papszAttrs[attrpos] != NULL)
		{
			std::wstring utf16AttrName,utf16AttrValue;
			xl::text::transcode::UTF8_to_Unicode(papszAttrs[attrpos],(xl::uint32)strlen(papszAttrs[attrpos]),utf16AttrName);
			xl::text::transcode::UTF8_to_Unicode(papszAttrs[attrpos+1],(xl::uint32)strlen(papszAttrs[attrpos+1]),utf16AttrValue);
			m_elements[m_parseDepth]->set_attribute(utf16AttrName.c_str(),utf16AttrValue.c_str());
			attrpos+=2;
		}
	}

	return;
}
template<>
inline void basic_xml_dom_parser<wchar_t>::OnCharacterData (const XML_Char *pszData, int nLength)
{
	if(m_currentCData == NULL)
	{
		m_currentCData = new basic_xml_cdata<wchar_t>();
	}
	
	std::wstring utf16CData;
	xl::text::transcode::UTF8_to_Unicode(pszData,nLength,utf16CData);
	m_currentCData->append_value(utf16CData.c_str(),utf16CData.size());
	return;
}

typedef basic_xml_dom_parser<char> xml_dom_parser;
typedef basic_xml_dom_parser<wchar_t> xml_dom_parser_utf16;

}//namespace XML

}//namespace Xunlei
#endif //_XUNLEI_DOM_PARSER_H_
