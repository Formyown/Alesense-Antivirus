/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2009                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   xml_element.h
*   Author      :   刘智聪
*   Create      :   2009年1月8日
*   LastChange  :   
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
*   FileName    :   XMLElement.hpp
*   Author      :   waterflier (waterflier@gmail.com)
*   Create      :   2006-12-17 13:57
*   LastChange  :   2009年1月8日
*   History     :	
*								
*   Description :   class "XMLElement",DOMParser得到的就是这种保存了
*                   所有信息的Element
*
********************************************************************/  


#ifndef _XUNLEI_ELEMENT_HPP_
#define _XUNLEI_ELEMENT_HPP_

#include <string>
#include <vector>

#include "./xml_attr.h"
#include "./xml_cdata.h"
#include "./xml_error.h"
#include "./xml_path.h"
#include "./xml_util.h"
#include "./xml_document.h"
#include "../text/transcode.h"
#include "../tchar.h"

namespace xl
{
namespace xml
{

template<typename T>
class basic_xml_element : public xml_node
{
public:
    basic_xml_element()
	{
		node_type = xml_node::XML_ELEMENT;
		m_attrList = NULL;
		m_father = NULL;
        m_owner_document = NULL;
	}

    basic_xml_element(const T* name,basic_xml_element<T>* pFather = NULL)
	{
		node_type = xml_node::XML_ELEMENT;
		assert(name);
		m_name = name;
        m_attrList = NULL;
		m_father = pFather;
        m_owner_document = NULL;
	}

    virtual ~basic_xml_element()
	{
		clean_cdata();
		
		if(m_attrList)
			m_attrList->release();
		
		m_father = NULL;
		if(m_owner_document)
        {
            m_owner_document->release();
            m_owner_document = NULL;
        }
		clean_child_elements();
	}

public:
	virtual size_t size_of()
	{
		size_t result = 0;
		result += sizeof(this);
		result += m_value.size() * sizeof(T);
		result += m_name.size() * sizeof(T);
		result += m_cacheCData.size() * sizeof(T);
		if(m_attrList)
		{
			result += m_attrList->size_of();
		}

		for(size_t j=0;j<m_cdatas.size();++j)
		{
			result += m_cdatas[j]->size_of();
		}

		for(size_t i=0;i<m_subs.size();++i)
		{
			result += m_subs[i]->size_of();
		}

		return result;
	}

	//调用SetValue会导致所有的CData丢失
    void set_value(const T* value)
	{
		if(m_isSaveValueInCData)
			clean_cdata();

		m_isSaveValueInCData = false;
		m_value = value;
	}
    void set_value(const T* value,size_t length)
	{
		if(m_isSaveValueInCData)
			clean_cdata();

		m_isSaveValueInCData = false;
		m_value.clear();
		m_value.append(value,length);
	}

	//调用AppendCData会导致SetValue丢失
	void append_cdata(basic_xml_cdata<T>* pCData)
	{
		m_isSaveValueInCData = true;
        m_isCacheMergedCData = false;
		pCData->add_ref();
		m_cdatas.push_back(pCData);
	}

    void clean_cdata()
	{
		for(size_t i=0;i<m_cdatas.size();++i)
		{
			if(m_cdatas[i])
			{
				m_cdatas[i]->release();
			}
		}
		m_cdatas.clear();
	}

	void set_name(const T* name)
	{
		m_name = name;
	}

    void merge_cdata()
    {
        if(!m_isSaveValueInCData)
            return;

        m_cacheCData.clear();
        for(size_t i=0;i<m_cdatas.size();++i)
        {
            const T* pCDataValue;
            size_t cdataValueLen;
            pCDataValue = m_cdatas[i]->get_value(&cdataValueLen);
            if(pCDataValue)
            {
                m_cacheCData.append(pCDataValue,cdataValueLen);
            }
        }
        m_isCacheMergedCData = true;
        return;   
    }

	const T* get_value()
	{
		//TODO:
		if(!m_isSaveValueInCData)
		{
			return m_value.c_str();
		}
        else
        {
            if(!m_isCacheMergedCData)
                merge_cdata();

            return m_cacheCData.c_str();
        }
	}

	const T* get_name() const
	{
		return m_name.c_str();
	}

	size_t get_cdata_count() const
	{
		return m_cdatas.size();
	}

	basic_xml_cdata<T>* get_cdata(size_t index) const
	{
		if(index < m_cdatas.size())
		{
			basic_xml_cdata<T>* pResult = m_cdatas[index];
			pResult->add_ref();
			return pResult;
		}
		return NULL;
	}

	void clean_attibute()
	{
		if(m_attrList)
		{
			m_attrList->reset();
			m_attrList->release();
			m_attrList = NULL;
		}
	}

    long set_attribute(const T* name,const T* value)
	{
		if(m_attrList == NULL)
		{
			m_attrList = new basic_xml_attr_list<T>();
		}
		m_attrList->set_attr(name,value);
        return XML_RESULT::RESULT_SUCCESS;
	}

	void set_attr_list(basic_xml_attr_list<T>* pAttr)
	{
		if(m_attrList)
			m_attrList->release();
		
		pAttr->add_ref();	
		m_attrList = pAttr;
	}

	basic_xml_attr_list<T>* get_attr_list()
	{
		if(m_attrList == NULL)
		{
			m_attrList = new basic_xml_attr_list<T>();
		}
		m_attrList->add_ref();
		return m_attrList;
	}

	const T* get_attribute_value(const T* name) const
	{
		if(m_attrList == NULL)
			return NULL;
		return m_attrList->get_attr_value(name);
	}

    static void update_child_document(basic_xml_element<T>* pThis)
    {
        xml_document* pDoc = pThis->get_document();
        for(size_t i=0;i<pThis->m_subs.size();++i)
        {
            if(pThis->m_subs[i])
            {
                pThis->m_subs[i]->set_document_imp(pDoc);
                update_child_document(pThis->m_subs[i]);
            }
            
        }
        if(pDoc)
        {
            pDoc->release();
        }
    }

    long add_child_element(basic_xml_element<T>* pChild)
	{
		assert(pChild);
		pChild->add_ref();
		m_subs.push_back(pChild);
        update_child_document(this);
        return XML_RESULT::RESULT_SUCCESS;
	}

    void clean_child_elements()
	{
		for(size_t i=0;i<m_subs.size();++i)
		{
			if(m_subs[i])
			{
				m_subs[i]->release();
			}
		}

		m_subs.clear();
	}

	void remove_child_element( size_t index )
	{
		if ( index < m_subs.size() )
		{
			if ( m_subs[ index ] )
			{
				m_subs[ index ]->release();
			}
			m_subs.erase( m_subs.begin() + index );
		}
	}

    basic_xml_element<T>* get_first_child_element(const T* name)
	{
		for(size_t i=0;i<m_subs.size();++i)
		{
			if(m_subs[i]->m_name == name)
			{
				m_subs[i]->add_ref();
				return m_subs[i];
			}
		}	

		return NULL;
	}

	size_t get_child_element_count() const
	{
		return m_subs.size();
	}

	basic_xml_element<T>* get_child_element(size_t index)
	{
		if(index < m_subs.size())
		{
			if(m_subs[index])
				m_subs[index]->add_ref();
			return m_subs[index];
		}

		return NULL;
	}

	void set_father(basic_xml_element<T>* father)
	{
		m_father = father;
	}

	basic_xml_element<T>* get_father()
	{
		if(m_father)
        {
			m_father->add_ref();
        }

		return m_father;
	}

    xml_document* get_document()
    {
        if(m_owner_document)
        {
            m_owner_document->add_ref();
        }
        return m_owner_document;
    }
    
    void set_document(xml_document* pdocument)
    {
        set_document_imp(pdocument);
        update_child_document(this);
    }

    //Select 函数负责提供对XPath的简单支持
    long select_first_node_value(const T* strXPath, const T* & result)
	{
		basic_xml_path<T> thePath;
		if(thePath.parser(strXPath) == XML_RESULT::RESULT_SUCCESS)
		{
			return select_first_node_value(thePath,result);
		}
		else
		{
			return XML_RESULT::INVALID_XPATH;
		}
	}

    long select_first_node_value(const basic_xml_path<T>& theXPath, const T* & result)
	{
		long retValue;
		basic_xml_element<T>* theNode = NULL;
		retValue = select_first_node(theXPath,theNode);
		if(retValue != XML_RESULT::RESULT_SUCCESS)
			return retValue;

		assert(theNode);
		bool isSelectAttribute = false;
		const T* attribute = theXPath.get_attribute_search_node();
		if(attribute != NULL)
		{
			if(attribute[0] != 0)
			{
				isSelectAttribute = true;
			}
		}    

		if(isSelectAttribute)
		{
			const T* pAttrValue = theNode->get_attribute_value(attribute);
			if(pAttrValue != NULL)
			{
				result = pAttrValue;
			}
			else
			{
				return XML_RESULT::CANNT_FIND_ATTRIBUTE;
			}
		}
		else
		{
			result = theNode->get_value();
		}

		return XML_RESULT::RESULT_SUCCESS;
	}

    long select_first_node(const T* strXPath,  basic_xml_element<T>* & result)
	{
		basic_xml_path<T> thePath;
		if(thePath.parser(strXPath) == XML_RESULT::RESULT_SUCCESS)
		{
			return select_first_node(thePath,result);
		}
		else
		{
			return XML_RESULT::INVALID_XPATH;
		}
	}

    long select_first_node(const basic_xml_path<T>& theXPath, basic_xml_element<T>* & result)
	{
		result = NULL;
		if(!theXPath.valid())
			return XML_RESULT::INVALID_XPATH;

		//const char* attribute = theXPath.GetAttributeSearchNode();
		//if(attribute != NULL)
		//{
		//    if(attribute[0] != 0)
		//    {
		//        return XML_RESULT::XPATH_CANNT_SELECT_ELEMENT;
		//    }
		//}

        const std::vector<T*>& searchNodes = theXPath.get_element_search_nodes();

		typename std::vector<T*>::const_iterator iter;
		iter = searchNodes.begin(); 
		if(theXPath.get_begin_search_depth() == 0)
		{    
			if(iter == searchNodes.end())
			{
				result = this;
				return XML_RESULT::INVALID_XPATH;
			}
			if(m_name != (*iter))
			{
				return XML_RESULT::XPATH_ROOT_ERROR;
			}
			++iter;
		}

		basic_xml_element<T>* pNow = this;
		for(;iter != searchNodes.end();++iter)
		{
			bool isFind = false;
			for(typename std::vector<basic_xml_element<T>*>::iterator iterChild = pNow->m_subs.begin();
				iterChild != pNow->m_subs.end();++iterChild)
			{
				if( (*iterChild)->m_name == (*iter))
				{
					pNow = (*iterChild);
					isFind = true;
					break;
				}
			}

			if(!isFind)
			{
				return XML_RESULT::CANNT_FIND_ELEMENT;
			} 
		}
		pNow->add_ref();
		result = pNow;
        return XML_RESULT::RESULT_SUCCESS;
	}
   
    inline void get_utf8_xml(std::string& strResult);


	bool write_utf8_xml_file(const tchar* pszPath)
    {

        FILE* fp;
		#ifdef __LINUX__
			fp = fopen(pszPath,("wb"));
		#else
			 fp = _tfopen(pszPath,TSTRING("wb"));
		#endif
       

        if(fp == NULL)
        {   
            return false;
        }

        std::string utf8XML;
        get_utf8_xml(utf8XML);
        fwrite(utf8XML.c_str(),1,utf8XML.size(),fp);
        fclose(fp);
        return true;
    }

protected:
    inline size_t get_utf8_xml_imp(char* pBuffer);
    inline size_t get_utf8_xml_len();

    void set_document_imp(xml_document* pdocument)
    {
        if(m_owner_document)
        {
            m_owner_document->release();
        }

        m_owner_document = pdocument;
        if(m_owner_document)
        {
            m_owner_document->add_ref();
        }

    }

protected:
    bool m_isSaveValueInCData;
    bool m_isCacheMergedCData;

	std::basic_string<T> m_value;
    std::basic_string<T> m_name;
    std::basic_string<T> m_cacheCData;

	std::vector<basic_xml_cdata<T>*> m_cdatas;
	basic_xml_attr_list<T>* m_attrList;

	basic_xml_element<T>* m_father;
    std::vector<basic_xml_element<T>*> m_subs;

    xml_document* m_owner_document;

protected:
    basic_xml_element(const basic_xml_element<T>& rhs)
    {
		return;
    }

    basic_xml_element<T>& operator=(const basic_xml_element<T>& rhs)
    {
        return *this;
    }
};

template<>
inline size_t basic_xml_element<wchar_t>::get_utf8_xml_len()
{
    size_t result = 0;
    result = (m_name.size() * 3 + 4)*2;
    if(m_attrList)
    {
        result += m_attrList->get_utf8_xml_len();
    }
    const wchar_t* pValue = get_value();
    if(pValue)
    {
        result += wcslen(pValue) * 5 + 4;
    }

    for(size_t i=0;i<m_subs.size();++i)
    {
        result += m_subs[i]->get_utf8_xml_len();
    }

    return result;
}



template<>
inline size_t basic_xml_element<wchar_t>::get_utf8_xml_imp (char* pBuffer)
{
    size_t nowWritePos = 0;
    size_t writeLen = 0;

    pBuffer[nowWritePos] = '<';
    nowWritePos ++;
    
    std::string utf8Name;
    xl::text::transcode::Unicode_to_UTF8(m_name.c_str(),(xl::uint32)m_name.size(),utf8Name);
    writeLen = xml_util::xml_string_copy(utf8Name.c_str(),pBuffer+nowWritePos);
    nowWritePos += writeLen;

    pBuffer[nowWritePos] = ' ';
    nowWritePos ++;

    if(m_attrList)
    {
        nowWritePos += m_attrList->get_utf8_xml(pBuffer+nowWritePos);
    }

    pBuffer[nowWritePos] = '>';
    nowWritePos ++;
    
    const wchar_t* ucsValue = get_value();
    std::string utf8Value;
    xl::text::transcode::Unicode_to_UTF8(ucsValue,(xl::uint32)wcslen(ucsValue),utf8Value);
    writeLen = xml_util::xml_string_copy(utf8Value.c_str(),pBuffer+nowWritePos);
    nowWritePos += writeLen;

    for(size_t i=0;i<m_subs.size();++i)
    {
        nowWritePos += m_subs[i]->get_utf8_xml_imp(pBuffer+nowWritePos);
    }

    if(m_subs.size() == 0)
    {
        if(utf8Value.length() == 0)
        {
            pBuffer[nowWritePos-1] = '/';
            pBuffer[nowWritePos] = '>';
            nowWritePos ++;
        }
        else
        {
            pBuffer[nowWritePos] = '<';
            nowWritePos ++;
            pBuffer[nowWritePos] = '/';
            nowWritePos ++;
            writeLen = xml_util::xml_string_copy(utf8Name.c_str(),pBuffer+nowWritePos);
            nowWritePos += writeLen;
            pBuffer[nowWritePos] = '>';
            nowWritePos ++;
        }
    }
    else
    {
        pBuffer[nowWritePos] = '<';
        nowWritePos ++;
        pBuffer[nowWritePos] = '/';
        nowWritePos ++;
        writeLen = xml_util::xml_string_copy(utf8Name.c_str(),pBuffer+nowWritePos);
        nowWritePos += writeLen;
        pBuffer[nowWritePos] = '>';
        nowWritePos ++;
    }

    return nowWritePos;
}

template<>
inline size_t basic_xml_element<char>::get_utf8_xml_len()
{
    size_t result = 0;
    result = (m_name.size() + 4)*2;
    if(m_attrList)
    {
        result += m_attrList->get_utf8_xml_len();
    }
    const char* pValue = get_value();
    if(pValue)
    {
        result += strlen(pValue)*5 + 4;
    }

    for(size_t i=0;i<m_subs.size();++i)
    {
        result += m_subs[i]->get_utf8_xml_len();
    }

    return result;
}



template<>
inline size_t basic_xml_element<char>::get_utf8_xml_imp (char* pBuffer)
{
    size_t nowWritePos = 0;
    size_t writeLen = 0;

    pBuffer[nowWritePos] = '<';
    nowWritePos ++;

    writeLen = xml_util::xml_string_copy(m_name.c_str(),pBuffer+nowWritePos);
    nowWritePos += writeLen;

    pBuffer[nowWritePos] = ' ';
    nowWritePos ++;

    std::string strAttr;
    if(m_attrList)
    {
        nowWritePos += m_attrList->get_utf8_xml(pBuffer + nowWritePos);
    }


    pBuffer[nowWritePos] = '>';
    nowWritePos ++;

    writeLen = xml_util::xml_string_copy(get_value(),pBuffer+nowWritePos);
    nowWritePos += writeLen;
    
    for(size_t i=0;i<m_subs.size();++i)
    {
        nowWritePos +=  m_subs[i]->get_utf8_xml_imp(pBuffer + nowWritePos);
    }
    
    if(m_subs.size() == 0)
    {
        if(strlen(get_value()) == 0)
        {
            pBuffer[nowWritePos-1] = '/';
            pBuffer[nowWritePos] = '>';
            nowWritePos ++;
        }
        else
        {
            pBuffer[nowWritePos] = '<';
            nowWritePos ++;
            pBuffer[nowWritePos] = '/';
            nowWritePos ++;
            writeLen = xml_util::xml_string_copy(m_name.c_str(),pBuffer+nowWritePos);
            nowWritePos += writeLen;
            pBuffer[nowWritePos] = '>';
            nowWritePos ++;
        }
    }
    else
    {
        pBuffer[nowWritePos] = '<';
        nowWritePos ++;
        pBuffer[nowWritePos] = '/';
        nowWritePos ++;
        writeLen = xml_util::xml_string_copy(m_name.c_str(),pBuffer+nowWritePos);
        nowWritePos += writeLen;
        pBuffer[nowWritePos] = '>';
        nowWritePos ++;
    }

    return nowWritePos;
}

template<>
inline void basic_xml_element<wchar_t>::get_utf8_xml (std::string& strResult)
{
	size_t bufferLen = get_utf8_xml_len();
	char* pBuffer = (char*) malloc(bufferLen);
	memset(pBuffer,0,bufferLen);
	get_utf8_xml_imp(pBuffer);
	strResult = pBuffer;
	free(pBuffer);
}

template<>
inline void basic_xml_element<char>::get_utf8_xml (std::string& strResult)
{
	size_t bufferLen = get_utf8_xml_len();
	char* pBuffer = (char*) malloc(bufferLen);
	memset(pBuffer,0,bufferLen);
	get_utf8_xml_imp(pBuffer);
	strResult = pBuffer;
	free(pBuffer);
}

typedef basic_xml_element<char> xml_element;
typedef basic_xml_element<wchar_t> xml_element_16;

}//namespace xml
}//namespace xl
#endif //_ELEMENT_HPP_
