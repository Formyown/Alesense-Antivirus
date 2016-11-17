/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2007                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   xml_node.h
*   Author      :   周蔚,刘智聪
*   Create      :   2007年8月1日
*   LastChange  :   
*   History     :	
*
*   Description :   XML Node
*
********************************************************************/ 

#ifndef __XUNLEI_XML_NODE_H_
#define __XUNLEI_XML_NODE_H_

#include <string>

#ifdef __LINUX__
#include "xl_lib/linux/multithread/spin_lock.h"
#endif

namespace xl
{

namespace xml
{


class xml_node
{
public:
    enum
    {
        XML_UNKNOW,
        XML_ELEMENT,
        XML_ATTR,
        XML_CDATA,
        XML_DOCUMENT
    } XMLNodeType;

public:
    xml_node(void)
#ifdef __LINUX__
    :m_lock()
#endif
    {
        m_ref = 1;
    }

    virtual ~xml_node(void)
    {
		return;
    }

public:

    unsigned long add_ref()
    {
#ifdef __LINUX__
        xl::lnx::multithread::glibc_spin_lock_guard lock(m_lock);
        m_ref++;

        return m_ref;
#else
        return InterlockedIncrement((LONG*)&m_ref);
#endif
        
    }

    unsigned long release()
    {
#ifdef __LINUX__
        // 这个地方不能用guard的原因在于：刘智聪在类对象中自己删除自己，shit
        //xl::lnx::multithread::glibc_spin_lock_guard lock(m_lock);
        m_lock.lock();
        m_ref--;

        unsigned long l = m_ref;
        m_lock.unlock();
#else
        unsigned long l = InterlockedDecrement((LONG*)&m_ref);
#endif        
        if(0 == l)
        {
            delete this;
        }

        return l;
    }
	
	virtual size_t size_of() = 0;
public:
    int node_type;

protected:
    unsigned long m_ref;

#ifdef __LINUX__
    xl::lnx::multithread::glibc_spin_lock m_lock;
#endif
};

}//xml
}//xl

#endif //__XUNLEI_XML_NODE_H_
