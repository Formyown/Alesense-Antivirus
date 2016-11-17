/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2007                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   xml_cdata.h
*   Author      :   周蔚,刘智聪
*   Create      :   2007年8月1日
*   LastChange  :   2009年1月8日
*   History     :	
*
*   Description :   XML CData
*
********************************************************************/ 

#ifndef __XML_CDATA_H_
#define __XML_CDATA_H_

#include <string>
#include "./xml_node.h"

namespace xl
{
namespace xml
{

template<typename T>
class basic_xml_cdata : public xml_node
{
public:
    basic_xml_cdata(void)
	{
		node_type = xml_node::XML_CDATA;
		return;
	}

    basic_xml_cdata(const T* pValue,size_t len)
	{
		node_type = xml_node::XML_CDATA;
		append_value(pValue,len);
	}

    virtual ~basic_xml_cdata(void)
	{
		return;
	}

public:
	virtual size_t size_of()
	{
		return sizeof(this) + m_value.size() * sizeof(T);
	}

	void clear()
	{
		m_value.clear();
	}

    void append_value(const T* pValue,size_t len)
	{
		m_value.append(pValue,len);
	}

    const T* get_value(size_t* pLen) const
	{
		if(pLen)
			*pLen = m_value.size();

		return m_value.c_str();
	}

	virtual void get_utf8_xml(std::string& strResult)
	{
		strResult = "";
	}

private:
    std::basic_string<T> m_value;

protected:
	basic_xml_cdata(const basic_xml_cdata& rhs){return;}
	basic_xml_cdata& operator=(const basic_xml_cdata& rhs){return;}
};

typedef basic_xml_cdata<char> xml_cdata;
typedef basic_xml_cdata<wchar_t> xml_cdata_utf16;

}//xml
}//xl

#endif
