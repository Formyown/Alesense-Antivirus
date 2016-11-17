/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2007                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   xml_attr.h
*   Author      :   周蔚,刘智聪
*   Create      :   2007年8月1日
*   LastChange  :   
*   History     :	
*
*   Description :   XML Attribute
*
********************************************************************/ 

#ifndef __XUNLEI_XML_ATTR_H_
#define __XUNLEI_XML_ATTR_H_

#include <string>
#include <vector>
#include <map>

#include "./xml_node.h"
#include "./xml_util.h"

#include "../text/transcode.h"

namespace xl
{


namespace xml
{

template<typename T> 
class basic_xml_attr_list : public xml_node
{
public:
    basic_xml_attr_list(void)
	{
		node_type = xml_node::XML_ATTR;
	}

    virtual ~basic_xml_attr_list(void)
	{
		return;
	}
public:
    typedef typename std::map<std::basic_string<T>,std::basic_string<T> >::iterator TIterator;
    
    TIterator begin()
    {
        return m_allAttr.begin();
    }

    TIterator end()
    {
        return m_allAttr.end();
    }

    bool remove_attr(TIterator iter)
    {
        if(iter == m_allAttr.end())
            return false;

        m_allAttr.erase(iter);
        return true;
    }

public:
	virtual size_t size_of()
	{
		size_t result = sizeof(this);
		TIterator iter = m_allAttr.begin();
		TIterator iterEnd = m_allAttr.end();
		for(;iter!=iterEnd;++iter)
		{
			result += 16;//
			result += sizeof(std::basic_string<T>)*2;
			result += iter->first.size() * sizeof(T);
			result += iter->second.size() * sizeof(T);
		}
		return result;
	}

	void set_attr(const T* pName,const T* pValue)
	{
		TIterator iterFind = m_allAttr.find(pName);
		if(iterFind == m_allAttr.end())
		{
			m_allAttr.insert(std::make_pair<std::basic_string<T>,std::basic_string<T> >(pName,pValue));
		}
		else
		{
			iterFind->second = pValue;
		}
	}
	
	const T* get_attr_value(const T* pName) const
	{
        typename std::map<std::basic_string<T>,std::basic_string<T> >::const_iterator iterFind;
		iterFind = m_allAttr.find(pName);
		if(iterFind == m_allAttr.end())
		{
			return NULL;
		}
		else
		{
			return iterFind->second.c_str();
		}
	}

    unsigned long get_attr_count() const
	{
		return (unsigned long)m_allAttr.size();
	}
	
	void reset()
	{
		m_allAttr.clear();
	}

	bool remove_attr(const T* pName)
	{
		typename std::map<std::basic_string<T>,std::basic_string<T> >::iterator iterFind;
		iterFind = m_allAttr.find(pName);
		if(iterFind == m_allAttr.end())
		{
			return false;
		}
		else
		{
			m_allAttr.erase(iterFind);
			return true;
		}
	}

	//TODO: 需要完善
	inline size_t get_utf8_xml(char* buffer);
    inline size_t get_utf8_xml_len();    

protected:
    std::map<std::basic_string<T>,std::basic_string<T> > m_allAttr;

private:
    basic_xml_attr_list(const basic_xml_attr_list& attrList);
    basic_xml_attr_list& operator=(const basic_xml_attr_list& attrList);
};

template<>
inline size_t basic_xml_attr_list<wchar_t>::get_utf8_xml_len ()
{
    size_t result = 0;
    std::map<std::basic_string<wchar_t>,std::basic_string<wchar_t> >::iterator iter,iterEnd;
    iterEnd = m_allAttr.end();
    for(iter = m_allAttr.begin();iter != iterEnd;++iter)
    {
        result += iter->first.size() * 4;
        result += iter->second.size() * 4;
        result += 4;
    }

    return result;
}


template<>
inline size_t basic_xml_attr_list<wchar_t>::get_utf8_xml (char* buffer)
{
    size_t nowWritePos = 0;
    size_t writeLen = 0;

    std::map<std::basic_string<wchar_t>,std::basic_string<wchar_t> >::iterator iter,iterEnd;
    iterEnd = m_allAttr.end();
    for(iter = m_allAttr.begin();iter != iterEnd;++iter)
    {
        std::string utf8Value;
        std::string utf8Name;
        xl::text::transcode::Unicode_to_UTF8(iter->first.c_str(),(xl::uint32)iter->first.size(),utf8Name);
        xl::text::transcode::Unicode_to_UTF8(iter->second.c_str(),(xl::uint32)iter->second.size(),utf8Value);
        writeLen = xml_util::xml_write_attr_value(utf8Name.c_str(),utf8Value.c_str(),buffer+nowWritePos,0x7fffffff);
        nowWritePos += writeLen;
    }

    return nowWritePos;
}

template<>
inline size_t basic_xml_attr_list<char>::get_utf8_xml_len ()
{
    size_t result = 0;
    std::map<std::basic_string<char>,std::basic_string<char> >::iterator iter,iterEnd;
    iterEnd = m_allAttr.end();
    for(iter = m_allAttr.begin();iter != iterEnd;++iter)
    {
        result += iter->first.size() * 3;
        result += iter->second.size() * 3;
        result += 4;
    }

    return result;
}

template<>
inline size_t basic_xml_attr_list<char>::get_utf8_xml (char* buffer)
{
    //TODO, 目前只提供一个临时的实现
    size_t nowWritePos = 0;
    size_t writeLen = 0;

    std::map<std::basic_string<char>,std::basic_string<char> >::iterator iter,iterEnd;
    iterEnd = m_allAttr.end();
    for(iter = m_allAttr.begin();iter != iterEnd;++iter)
    {
        writeLen = xml_util::xml_write_attr_value(iter->first.c_str(),iter->second.c_str(),buffer+nowWritePos,0x7fffffff);
        nowWritePos += writeLen;
    }

    return nowWritePos;
}

typedef basic_xml_attr_list<char> xml_attr_list;
typedef basic_xml_attr_list<wchar_t> xml_attr_list_utf16;

}//XML
}//Xunlei

#endif//__XUNLEI_XML_ATTR_H_
