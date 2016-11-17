/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, LTD. 2009                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   xml_path.h 
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
* =                       /  _   \ ● /    \ / /                    =
* =                      /  /_/  // // / / // /                     =
* =                      \______//_//_/ /_//_/                      =
* =                                                                 =
* =             Copyright (c) BIN Technology studio,2004 - 2008     =
* =                           LET'Z BT                              =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XPath.hpp (<bint/xml/XPath.hpp>)
*   Author      :   waterflier (waterflier@gmail.com)
*   Create      :   2006-12-17 13:57
*   LastChange  :   2006-12-17 13:57
*   History     :	1) 基本完成
                    2006-12-18 11:24 
*								
*   Description :   class "XPath",对于W3C XPath的一个简单实现,可以用于
*                   在XMLElement中查找子Element的值或某个属性的值
*                   目前支持的形式:
*                   /thiNodeName/childNodeName 查找名为childNodeName子节点的值
*                   childNodeName 查找名为childNodeName子节点的值
*                   childNodeName/@attribute 查找名为childNodeName子节点的attribute属性的值
*
********************************************************************/  

#ifndef _XUNLEI_XMLPATH_HPP_
#define _XUNLEI_XMLPATH_HPP_

#include <string>
#include <vector>

#include "./xml_error.h"

namespace xl
{
namespace xml
{

template<typename T>
class basic_xml_path
{
public:
    basic_xml_path(const T* strXPath = NULL)
	{
        m_buffer = NULL;
        m_attributeNodeBuffer = NULL;
        m_beginSearchDepth = 0;
        m_valid = false;    
		if(strXPath != NULL)
		{
			parser(strXPath);
		}
	}

    virtual ~basic_xml_path()
	{
		if(m_buffer)
			delete[] m_buffer;
		m_buffer = NULL;
	}

public:
    long parser(const T* strXPath)
	{
		assert(strXPath);
		assert(strXPath);

		m_strXPath = strXPath;
		clean();

		int state = 0;
		m_buffer = new T[m_strXPath.size() * 2 + 8];
		T* nowBuffer = m_buffer;
		memset(m_buffer,0,m_strXPath.size() * 2 * sizeof(T) + 8);
		int nowWritePos = 0;

		if(strXPath[0] == '/')
		{
			m_beginSearchDepth = 0;
		}
		else if(strXPath[0] == '@')
		{
			m_beginSearchDepth = 0;
			state = 1;
		}
		else 
		{
			if(is_valid_char(strXPath[0]))
			{
				m_beginSearchDepth = 1;
				nowBuffer[nowWritePos++] = strXPath[0];
			}
			else
			{
				return XML_RESULT::INVALID_XPATH;
			}
		}


		for(size_t i=1;i<m_strXPath.size();++i)
		{
			T nowChar = strXPath[i];
			switch(nowChar)
			{
			case '/':
				if(state == 0)
				{
					nowBuffer[nowWritePos] = 0;
					m_searchNodes.push_back(nowBuffer);
					nowBuffer = nowBuffer + nowWritePos + 1;
					nowWritePos = 0;
				}
				else
				{   
					return XML_RESULT::INVALID_XPATH;
				}
				break;
			case '@':
				if(nowWritePos != 0)
				{
					return XML_RESULT::INVALID_XPATH;
				}
				if(state == 0)
				{
					state = 1;
				}
				else
				{   
					return XML_RESULT::INVALID_XPATH;
				}
				break;
			default:
				if(is_valid_char(nowChar))
				{
					nowBuffer[nowWritePos++] = nowChar;
				}
				else
				{
					return XML_RESULT::INVALID_XPATH;
				}
			}   
		}

		if(nowWritePos == 0)
		{
			return XML_RESULT::INVALID_XPATH;
		}

		if(state == 0)
		{
			nowBuffer[nowWritePos] = 0;
			m_searchNodes.push_back(nowBuffer);
		}
		else 
		{
			nowBuffer[nowWritePos] = 0;
			m_attributeNodeBuffer = nowBuffer;
		}

		m_valid = true;
		return XML_RESULT::RESULT_SUCCESS;
	}

    long parser(const T* strXPath,size_t XPathLength)
	{
		assert(strXPath);
		assert(XPathLength >=0 );

		m_strXPath.erase();
		m_strXPath.append(strXPath,XPathLength);	
		return parser(m_strXPath.c_str());
	}

    bool valid() const
	{
		return m_valid;
	}
    
    long  get_begin_search_depth() const
	{
		return m_beginSearchDepth;
	}
    const T* get_xpath_string() const 
	{
		return m_strXPath.c_str();
	}

    const T* get_attribute_search_node() const
	{
		return m_attributeNodeBuffer;
	}

    const std::vector<T*>& get_element_search_nodes() const
	{
		assert(m_valid); 
		return m_searchNodes;
	}

protected:
    void clean()
	{
		m_valid = false;
		if(m_buffer)
		{
			delete[] m_buffer;
			m_buffer = NULL;
		}
		m_attributeNodeBuffer = NULL;

		m_searchNodes.clear();
		m_beginSearchDepth = 0;
	}

    inline bool is_valid_char(T ch) const
	{
		//考虑 ch可能是UTF8编码的一部分
		if(ch < 0)
			return true;

		if(ch >0)
		{
			if( (ch >= '0' && ch <='9') ||
				(ch >= 'A' && ch <= 'Z') ||
				(ch >= 'a' && ch <= 'z') ||
				(ch == '_') )
			{
				return true;
			}
		}

		return false;
	}

    bool m_valid;
   
    long m_beginSearchDepth;
    std::basic_string<T> m_strXPath;
    T* m_attributeNodeBuffer;
    T* m_buffer;
    std::vector<T*> m_searchNodes; 
    
protected:
    basic_xml_path(const basic_xml_path& rhs)
    {
    }

    basic_xml_path& operator=(const basic_xml_path& rhs)
    {
        return *this;
    }
};

typedef basic_xml_path<char> xml_path;
typedef basic_xml_path<wchar_t> xml_path_utf16;

}//namespace xml

}//namespace xl

#endif //_XUNLEI_XMLPATH_HPP_
